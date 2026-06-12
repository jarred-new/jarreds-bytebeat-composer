package com.jarredapps.bytebeat;

public class Token {

    public static final int EOF = 0;
    public static final int NUMBER = 1;
    public static final int IDENT = 2;
    public static final int OP = 3;

    public int type;
    public String text;

    public Token(int type, String text) {
        this.type = type;
        this.text = text;
    }

    @Override
    public String toString() {
        return text;
    }
}
