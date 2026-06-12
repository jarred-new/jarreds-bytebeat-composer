package com.jarredapps.bytebeat;

public class UnaryNode extends Node {

    private String op;
    private Node child;

    public UnaryNode(String op, Node child) {

        this.op = op;
        this.child = child;
    }

    @Override
    public double eval(long t) {

        double v = child.eval(t);

        if(op.equals("-"))
            return -v;

        if(op.equals("+"))
            return v;

        if(op.equals("!"))
            return (v == 0) ? 1 : 0;

        if(op.equals("~"))
            return ~((long)v);

        return 0;
    }
}
