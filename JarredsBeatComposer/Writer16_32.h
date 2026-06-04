// Writer16_32 (a.k.a BeatC Engine)
// Created by: Jarred
#pragma once
#include <afx.h>

extern "C" {
#include <stdio.h>
#include <math.h>
}


class CWriter16_32
{
public:
	CWriter16_32();
	~CWriter16_32();

public:
	// Helper to write 32-bit little-endian integers
	void put32(unsigned int n) {
		putchar(n & 0xFF);
		putchar((n >> 8) & 0xFF);
		putchar((n >> 16) & 0xFF);
		putchar((n >> 24) & 0xFF);
	}

	// Helper to write 16-bit little-endian integers
	void put16(unsigned short n) {
		putchar(n & 0xFF);
		putchar((n >> 8) & 0xFF);
	}
};

