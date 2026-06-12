package com.jarredapps.bytebeat;

public class BinaryNode extends Node {

    private String op;

    private Node left;
    private Node right;

    public BinaryNode(
        String op,
        Node left,
        Node right) {

        this.op = op;
        this.left = left;
        this.right = right;
    }

    @Override
    public double eval(long t) {

        double a = left.eval(t);
        double b = right.eval(t);

        long la = (long)a;
        long lb = (long)b;

        if(op.equals("+")) return a+b;
        if(op.equals("-")) return a-b;
        if(op.equals("*")) return a*b;
        if(op.equals("/")) return b==0 ? 0 : a/b;
        if(op.equals("%")) return b==0 ? 0 : a%b;

        if(op.equals("&")) return la&lb;
        if(op.equals("|")) return la|lb;
        if(op.equals("^")) return la^lb;

        if(op.equals("<<")) return la<<lb;
        if(op.equals(">>")) return la>>lb;
        if(op.equals(">>>")) return la>>>lb;

        if(op.equals("<")) return a<b ? 1:0;
        if(op.equals(">")) return a>b ? 1:0;

        if(op.equals("<=")) return a<=b ? 1:0;
        if(op.equals(">=")) return a>=b ? 1:0;

        if(op.equals("==")) return a==b ? 1:0;
        if(op.equals("!=")) return a!=b ? 1:0;

        if(op.equals("&&"))
            return (a!=0 && b!=0)?1:0;

        if(op.equals("||"))
            return (a!=0 || b!=0)?1:0;

        return 0;
    }
}
