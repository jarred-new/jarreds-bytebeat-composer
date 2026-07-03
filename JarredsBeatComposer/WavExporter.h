#pragma once
#include <afx.h>

extern "C" {
#include <stdio.h>
#include <math.h>
}

class CBytebeatEngine;

class CWavExporter
{
public:
	static bool Export(LPCTSTR path, 
		CBytebeatEngine& engine, 
		int sampleRate, 
		int seconds, 
		bool isCEngine, 
		int isSigned);
};