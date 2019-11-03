#include <string>
#include <string_view>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

class Token
{
public:
    enum class Type
    {
        Identifier,
        Number,
        Comment,
        LParent,
        RParent,
        LSqParent,
        RSqParent,
        LCurly,
        RCurly,
        Less,
        More,
        Equal,
        NotEqual,
        GreaterThan,
        Plus,
        Minus,
        Asterisk,
        Slash,
        Comma,
        Dot1,
        Dot2,
        Dot3,
        Semicolon,
        At,
        Hash,
        Tilda,
        Colon,
        Colon2,
        Dollar,
        QuestionMark,
        Up,
        UpEqual,
        Mod,
        ModEqual,
        WhatIsThatRyadovoyKucha,
        EndOfCode,
        StringLiteral,
        Keyword
    };

    Type m_type{};
    std::string m_lexema{};

    Token(Type type) : m_type{type} {}
    Token(Type type, char *beg, std::size_t len) : m_type{type}, m_lexema(beg, len) {}
    Token(Type type, char *beg, char *end) : m_type{type}, m_lexema(beg, std::distance(beg, end)) {}

    Type type() { return m_type; }
    void type(Type type) { m_type = type; }

    bool is(Type type) { return type == m_type; }
    bool is_not(Type type) { return type != m_type; }

    bool is_one_of(Type t1, Type t2) { return is(t1) || is(t2); }

    template <typename... Ts>
    bool is_one_of(Type t1, Type t2, Ts... ts)
    {
        return is(t1) || is_one_of(t2, ts...);
    }

    std::string lexeme() { return m_lexema; }
    void lexeme(std::string lexeme) { m_lexema = lexeme; }
};

class Lexer
{
public:
    char *m_start = nullptr;

    Lexer(char *start) : m_start{start} {}
    Token next();

    Token identifier();
    Token number();
    Token slash_comment();
    Token string_literal();
    Token keyword(char*, char*);
    Token atom(Token::Type);

    char peek() { return *m_start; }
    char get() { return *m_start++; }
    char reset() { return *m_start--; }
};

bool is_space(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return true;
    default:
        return false;
    }
}

bool is_number(char c)
{
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

bool is_identifier(char c)
{
    if ((c >= 'a' && c<= 'z') || (c >= 'A' && c <= 'Z') || (c >= '1' && c <= '9') || c == '_')
        return true;
    else
        return false;
}

Token Lexer::atom(Token::Type type) { return Token(type, m_start++, 1); }

Token Lexer::next()
{
    char *start;

    while (is_space(peek()))
        get();


    switch (peek())
    {
    case '\0':
        return Token(Token::Type::EndOfCode, m_start, 1);
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
        return identifier();
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return number();
    case '(':
        return atom(Token::Type::LParent);
    case ')':
        return atom(Token::Type::RParent);
    case '[':
        return atom(Token::Type::LSqParent);
    case ']':
        return atom(Token::Type::RSqParent);
    case '{':
        return atom(Token::Type::LCurly);
    case '}':
        return atom(Token::Type::RCurly);
    case '.':
        return atom(Token::Type::Dot1);
    case ';':
        return atom(Token::Type::Semicolon);
    case ',':
        return atom(Token::Type::Comma);
    case ':':
        start = m_start;
        get();
        switch (peek())
        {
        case '=':
            get();
            return Token(Token::Type::Colon2, start, m_start);
        default:
            reset();
            return atom(Token::Type::Colon);
        }

}

Token Lexer::identifier()
{
    char *start = m_start;
    get();
    while (is_identifier(peek()))
        get();
    return keyword(start, m_start);
}

Token Lexer::keyword(char* start, char* end) {
    std::string lexeme(start, end);

    if (
            lexeme == "class" ||
            lexeme == "is" ||
            lexeme == "end" ||
            lexeme == "extends" ||
            lexeme == "this" ||
            lexeme == "return" ||
            lexeme == "if" ||
            lexeme == "then" ||
            lexeme == "else" ||
            lexeme == "while" ||
            lexeme == "loop" ||
            lexeme == "method"
        ) {
        return Token(Token::Type::Keyword, start, end);
    } else {
        return Token(Token::Type::Identifier, start, end);
    }
}

Token Lexer::number()
{
    char *start = m_start;
    get();
    while (is_number(peek()))
        get();
    return Token(Token::Type::Number, start, m_start);
}

Token Lexer::slash_comment()
{
    char *start = m_start;
    get();
    if (peek() == '/')
    {
        get();
        start = m_start;
        while (peek() != '\0')
        {
            if (get() == '\n')
            {
                return Token(Token::Type::Comment, start, std::distance(start, m_start) - 1);
            }
        }
        return Token(Token::Type::WhatIsThatRyadovoyKucha, m_start, 1);
    }
    reset();
    return atom(Token::Type::Slash);
}

Token Lexer::string_literal()
{
    char* start = m_start;
    get();
    while(peek() != '\0') {
        if(get() == '\"') {
            return Token(Token::Type::StringLiteral, start, m_start);
        }
    }
    return Token(Token::Type::WhatIsThatRyadovoyKucha, m_start, 1);
}

#include <cstring>

std::ostream &operator<<(std::ostream &os, const Token::Type &type)
{
    static std::string names[]{
        "IDENTIFIER",
        "NUMBER",
        "COMMENT",
        "LPAREN",
        "RPAREN",
        "LBRACKET",
        "RBRACKET",
        "LBRACE",
        "RBRACE",
        "COMMA",
        "DOT",
        "DOUBLEDOT",
        "STRINGLITERAL",
        "KEYWORD"
        };
    return os << names[static_cast<int>(type)];
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: main <filepath>\n";
    }
    std::string filename = argv[1];

    std::ifstream fin;
    fin.open(filename);

    std::string str;
    std::ostringstream ss;

    ss << fin.rdbuf();
    str = ss.str();

    char input[str.length()];
    strcpy(input, str.c_str());

    Lexer lexer(input);

    std::ofstream fout;
    fout.open("tokens.txt");

    for (auto token = lexer.next();
         not token.is_one_of(Token::Type::EndOfCode, Token::Type::WhatIsThatRyadovoyKucha);
         token = lexer.next())
    {
        fout << std::setw(12) << token.type() << " |" << token.lexeme()
                  << "|\n";
    }

    fout.close();
}
