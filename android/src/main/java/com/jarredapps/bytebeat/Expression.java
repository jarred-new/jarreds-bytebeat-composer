package com.jarredapps.bytebeat;

public class Expression {

    private Node root;

    public Expression(String formula) {
        Parser parser = new Parser(formula);
        root = parser.parse();
    }

    public double evaluate(long t) {
        return root.eval(t);
    }
}
