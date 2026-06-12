package com.jarredapps.bytebeat;

import java.util.ArrayList;
import java.util.List;
import android.util.Log;

public class Parser {

    private Lexer lexer;
    private Token token;
    
    private String TAG = "PARSER";

    public Parser(String text) {

        lexer = new Lexer(text);

		token = lexer.nextToken();

		Log.d(TAG,
			  token.type + " " + token.text);
    }

    private void next() {
        token =
            lexer.nextToken();
    }

    private boolean match(String s) {

        if(token.text.equals(s)) {

            next();

            return true;
        }

        return false;
    }

    public Node parse() {

        Node n =
            parseTernary();

        if(token.type != Token.EOF)
            throw new RuntimeException(
                "Unexpected token: "
                + token.text);

        return n;
    }
    
    private Node parsePrimary() {

		//----------------------------------
		// Parentheses
		//----------------------------------

		if(match("(")) {

			Node n = parseTernary();

			if(!match(")"))
				throw new RuntimeException(
					") expected");

			return n;
		}

		//----------------------------------
		// Number
		//----------------------------------

		Log.d(TAG,
			  "TOKEN = [" +
			  token.text +
			  "] TYPE=" +
			  token.type);

		if(token.type == Token.NUMBER) {

			String text = token.text;

			next();

			if(text.startsWith("0x")
			   || text.startsWith("0X")) {

				return new NumberNode(
					Long.parseLong(
						text.substring(2),
						16));
			}

			return new NumberNode(
				Double.parseDouble(text.trim()));
		}

		//----------------------------------
		// Identifier
		//----------------------------------

		if(token.type == Token.IDENT) {

			String name = token.text;

			next();

			if(token.text.equals("(")) {

				match("(");

				List<Node> args =
					new ArrayList<Node>();

				if(!match(")")) {

					do {
						args.add(parseTernary());
					} while(match(","));

					if(!match(")"))
						throw new RuntimeException(
							") expected");
				}

				return new FunctionNode(
					name,
					args);
			}

			if(name.equals("t"))
				return new VariableNode();

			if(name.equalsIgnoreCase("PI"))
				return new NumberNode(Math.PI);

			if(name.equalsIgnoreCase("E"))
				return new NumberNode(Math.E);

			if(name.equalsIgnoreCase("sr"))
				return new NumberNode(8000);

			if(name.equalsIgnoreCase("rate"))
				return new NumberNode(8000);

			throw new RuntimeException(
				"Unknown identifier: "
				+ name);
		}

		throw new RuntimeException(
			"Unexpected token: "
			+ token.text);
	}
    
    private Node parseUnary() {

        if(match("+"))
            return new UnaryNode(
                "+",
                parseUnary());

        if(match("-"))
            return new UnaryNode(
                "-",
                parseUnary());

        if(match("!"))
            return new UnaryNode(
                "!",
                parseUnary());

        if(match("~"))
            return new UnaryNode(
                "~",
                parseUnary());

        return parsePrimary();
    }
    
    private Node parseAdd() {

        Node left =
            parseMultiply();

        while(true) {

            if(match("+"))
                left =
                    new BinaryNode(
                    "+",
                    left,
                    parseMultiply());

            else if(match("-"))
                left =
                    new BinaryNode(
                    "-",
                    left,
                    parseMultiply());

            else
                break;
        }

        return left;
    }
    
    
    private Node parseMultiply() {

        Node left =
            parseUnary();

        while(true) {

            if(match("*"))
                left =
                    new BinaryNode(
                    "*",
                    left,
                    parseUnary());

            else if(match("/"))
                left =
                    new BinaryNode(
                    "/",
                    left,
                    parseUnary());

            else if(match("%"))
                left =
                    new BinaryNode(
                    "%",
                    left,
                    parseUnary());

            else
                break;
        }

        return left;
    }
    
    private Node parseShift() {

        Node left =
            parseAdd();

        while(true) {

            if(match("<<"))
                left =
                    new BinaryNode(
                    "<<",
                    left,
                    parseAdd());

            else if(match(">>"))
                left =
                    new BinaryNode(
                    ">>",
                    left,
                    parseAdd());

            else if(match(">>>"))
                left =
                    new BinaryNode(
                    ">>>",
                    left,
                    parseAdd());

            else
                break;
        }

        return left;
    }
    
    private Node parseTernary() {

        Node condition =
            parseLogicalOr();

        if(match("?")) {

            Node trueNode =
                parseTernary();

            if(!match(":"))
                throw new RuntimeException(
                    ": expected");

            Node falseNode =
                parseTernary();

            return new TernaryNode(
                condition,
                trueNode,
                falseNode);
        }

        return condition;
    }
    
    private Node parseRelational() {

        Node left = parseShift();

        while(true) {

            if(match("<="))
                left = new BinaryNode(
                    "<=",
                    left,
                    parseShift());

            else if(match(">="))
                left = new BinaryNode(
                    ">=",
                    left,
                    parseShift());

            else if(match("<"))
                left = new BinaryNode(
                    "<",
                    left,
                    parseShift());

            else if(match(">"))
                left = new BinaryNode(
                    ">",
                    left,
                    parseShift());

            else
                break;
        }

        return left;
    }
    
    private Node parseEquality() {

        Node left = parseRelational();

        while(true) {

            if(match("=="))
                left = new BinaryNode(
                    "==",
                    left,
                    parseRelational());

            else if(match("!="))
                left = new BinaryNode(
                    "!=",
                    left,
                    parseRelational());

            else
                break;
        }

        return left;
    }
    
    private Node parseBitwiseAnd() {

        Node left = parseEquality();

        while(match("&")) {

            left = new BinaryNode(
                "&",
                left,
                parseEquality());
        }

        return left;
    }
    
    private Node parseBitwiseXor() {

        Node left = parseBitwiseAnd();

        while(match("^")) {

            left = new BinaryNode(
                "^",
                left,
                parseBitwiseAnd());
        }

        return left;
    }
    
    private Node parseBitwiseOr() {

        Node left = parseBitwiseXor();

        while(match("|")) {

            left = new BinaryNode(
                "|",
                left,
                parseBitwiseXor());
        }

        return left;
    }
    
    private Node parseLogicalAnd() {

        Node left = parseBitwiseOr();

        while(match("&&")) {

            left = new BinaryNode(
                "&&",
                left,
                parseBitwiseOr());
        }

        return left;
    }
    
    private Node parseLogicalOr() {

        Node left = parseLogicalAnd();

        while(match("||")) {

            left = new BinaryNode(
                "||",
                left,
                parseLogicalAnd());
        }

        return left;
    }
}
