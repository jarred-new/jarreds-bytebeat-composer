package com.jarredapps.bytebeat;

import java.util.List;

public class FunctionNode extends Node {

    private String name;
    private List<Node> args;

    public FunctionNode(
        String name,
        List<Node> args) {

        this.name = name.toLowerCase();
        this.args = args;
    }

    @Override
    public double eval(long t) {

        if(name.equals("sin"))
            return Math.sin(args.get(0).eval(t));

        if(name.equals("cos"))
            return Math.cos(args.get(0).eval(t));

        if(name.equals("tan"))
            return Math.tan(args.get(0).eval(t));

        if(name.equals("sqrt"))
            return Math.sqrt(args.get(0).eval(t));

        if(name.equals("abs"))
            return Math.abs(args.get(0).eval(t));

        if(name.equals("floor"))
            return Math.floor(args.get(0).eval(t));

        if(name.equals("ceil"))
            return Math.ceil(args.get(0).eval(t));

        if(name.equals("log"))
            return Math.log(args.get(0).eval(t));

        if(name.equals("exp"))
            return Math.exp(args.get(0).eval(t));

        if(name.equals("pow"))
            return Math.pow(
                args.get(0).eval(t),
                args.get(1).eval(t));

        if(name.equals("min"))
            return Math.min(
                args.get(0).eval(t),
                args.get(1).eval(t));

        if(name.equals("max"))
            return Math.max(
                args.get(0).eval(t),
                args.get(1).eval(t));

        if(name.equals("rand"))
            return Math.random();

        return 0;
    }
}
