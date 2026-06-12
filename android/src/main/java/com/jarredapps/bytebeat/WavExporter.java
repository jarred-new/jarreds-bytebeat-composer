package com.jarredapps.bytebeat;

import java.io.OutputStream;

public class WavExporter {

    public static void export(
        String formula,
        final BytebeatType type,
        OutputStream out,
        int sampleRate,
        int seconds)
    throws Exception {

        int samples =
            sampleRate * seconds;

        Expression expr =
            new Expression(formula);

        int dataSize =
            samples * 2;

        writeHeader(
            out,
            sampleRate,
            dataSize);

        for(long t = 0; t < samples; t++) {

            double value =
                expr.evaluate(t);

            short s = 0;

            if (type == BytebeatType.BYTEBEAT_SIGNED) {
                s =
                    (short)(((long)value & 255) * 256);
            } else if (type == BytebeatType.BYTEBEAT_UNSIGNED) {
                s =
                    (short)((value - 128) * 256);
            }

            out.write(s & 0xFF);
            out.write((s >> 8) & 0xFF);
        }

        out.flush();
        //out.close();
    }

    private static void writeHeader(
        OutputStream out,
        int sampleRate,
        int dataSize)
    throws Exception {

        writeString(out,"RIFF");
        writeInt(out,36 + dataSize);
        writeString(out,"WAVE");

        writeString(out,"fmt ");
        writeInt(out,16);

        writeShort(out,(short)1);
        writeShort(out,(short)1);

        writeInt(out,sampleRate);
        writeInt(out,sampleRate * 2);

        writeShort(out,(short)2);
        writeShort(out,(short)16);

        writeString(out,"data");
        writeInt(out,dataSize);
    }

    private static void writeString(
        OutputStream out,
        String s)
    throws Exception {

        out.write(s.getBytes("ASCII"));
    }

    private static void writeInt(
        OutputStream out,
        int v)
    throws Exception {

        out.write(v & 0xFF);
        out.write((v >> 8) & 0xFF);
        out.write((v >> 16) & 0xFF);
        out.write((v >> 24) & 0xFF);
    }

    private static void writeShort(
        OutputStream out,
        short v)
    throws Exception {

        out.write(v & 0xFF);
        out.write((v >> 8) & 0xFF);
    }
}
