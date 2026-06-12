package com.jarredapps.bytebeat;

public class NumberNode extends Node {

    private double value;

    public NumberNode(double value) {
        this.value = value;
    }

    @Override
    public double eval(long t) {
        return value;
    }
}
