import java.util.regex.Matcher;
import java.util.regex.Pattern;

public enum Token {
    ASSIGN (":="),
    LPAREN ("\\("),
    RPAREN ("\\)"), 
    COMMA (","),
    LBRACE ("\\{"),
    RBRACE ("\\}"),
    DOT ("\\."),
    LBRACKET ("\\[")
    RBRACKET ("\\]")

    CLASS ("class"),
    IS ("is"),
    IF ("if"), 
    THEN ("then"), 
    ELSE ("else"), 
    IS ("is"),
    THIS ("this"),
    RETURN ("return"),
    WHILE ("while"),
    TRUE ("true"),
    LOOP ("loop"),
    FALSE ("false"),
    METHOD ("method"),
    VAR ("var"),
    EXTENDS ("extends"),

    STRING ("\"[^\"]+\""),
    INTEGER ("\\d"), 
    IDENTIFIER ("\\w+");

    private final Pattern pattern;

    Token(String regex) {
        pattern = Pattern.compile("^" + regex);
    }

    int endOfMatch(String s) {
        Matcher m = pattern.matcher(s);

        if (m.find()) {
            return m.end();
        }
        return -1;
    }
}