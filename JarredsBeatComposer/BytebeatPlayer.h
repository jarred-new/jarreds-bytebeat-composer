#pragma once
#include <windows.h>
#include <mmsystem.h>

#include "BytebeatEngine.h"

#pragma comment(lib, "winmm.lib")

class CBytebeatPlayer
{
public:

	CBytebeatPlayer();
	~CBytebeatPlayer();

	bool Start(CBytebeatEngine* engine, int sampleRate);
	void Stop();

	bool IsPlaying() const;

private:

	static DWORD WINAPI AudioThreadProc(
		LPVOID param);

	void AudioLoop();

private:

	CBytebeatEngine* m_engine;

	HANDLE m_thread;
	bool m_running;

	int m_sampleRate;
	uint32_t m_t;
};