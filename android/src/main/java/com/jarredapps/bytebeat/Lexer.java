package com.jarredapps.bytebeat;

public class Lexer {

    private String input;
    private int pos;

    public Lexer(String input) {
        this.input = input;
    }

    private char peek() {

        if(pos >= input.length())
            return '\0';

        return input.charAt(pos);
    }

    private char next() {

        if(pos >= input.length())
            return '\0';

        return input.charAt(pos++);
    }

    public Token nextToken() {

        while(Character.isWhitespace(peek()))
            next();

        char c = peek();

        if(c == '\0')
            return new Token(Token.EOF,"");

        //----------------------------------
        // Number
        //----------------------------------

        if(Character.isDigit(c)) {

            StringBuilder sb =
                new StringBuilder();

            if(c == '0'
               && pos+1 < input.length()
               && (input.charAt(pos+1)=='x'
               || input.charAt(pos+1)=='X')) {

                sb.append(next());
                sb.append(next());

                while(
                    Character.digit(
                        peek(),16) != -1)
                    sb.append(next());

                return new Token(
                    Token.NUMBER,
                    sb.toString());
            }

            while(
                Character.isDigit(peek())
                || peek()=='.')
                sb.append(next());

            return new Token(
                Token.NUMBER,
                sb.toString());
        }

        //----------------------------------
        // Identifier
        //----------------------------------

        if(Character.isLetter(c)
           || c=='_') {

            StringBuilder sb =
                new StringBuilder();

            while(
                Character.isLetterOrDigit(peek())
                || peek()=='_')
                sb.append(next());

            return new Token(
                Token.IDENT,
                sb.toString());
        }

        //----------------------------------
        // 3-char operators
        //----------------------------------

        if(pos+2 < input.length()) {

            String s =
                input.substring(
                pos,
                pos+3);

            if(s.equals(">>>")) {

                pos += 3;

                return new Token(
                    Token.OP,
                    s);
            }
        }

        //----------------------------------
        // 2-char operators
        //----------------------------------

        if(pos+1 < input.length()) {

            String s =
                input.substring(
                pos,
                pos+2);

            if(
                s.equals("<<")
                || s.equals(">>")
                || s.equals("<=")
                || s.equals(">=")
                || s.equals("==")
                || s.equals("!=")
                || s.equals("&&")
                || s.equals("||")) {

                pos += 2;

                return new Token(
                    Token.OP,
                    s);
            }
        }

        //----------------------------------
        // single char
        //----------------------------------

        next();

        return new Token(
            Token.OP,
            String.valueOf(c));
    }
}
