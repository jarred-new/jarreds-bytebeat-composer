package com.jarredapps.bytebeat;

public class TernaryNode extends Node {

    private Node condition;
    private Node trueNode;
    private Node falseNode;

    public TernaryNode(
        Node condition,
        Node trueNode,
        Node falseNode) {

        this.condition = condition;
        this.trueNode = trueNode;
        this.falseNode = falseNode;
    }

    @Override
    public double eval(long t) {

        if(condition.eval(t) != 0)
            return trueNode.eval(t);

        return falseNode.eval(t);
    }
}
