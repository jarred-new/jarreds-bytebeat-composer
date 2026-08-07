#include "stdafx.h"
#include "BytebeatPlayer.h"

CBytebeatPlayer::CBytebeatPlayer()
{
	m_engine = NULL;
	m_thread = NULL;
	m_running = false;
	m_sampleRate = 8000;
	//m_sampleRate = 44100;
	m_t = 0;
}

CBytebeatPlayer::~CBytebeatPlayer()
{
	Stop();
}

bool CBytebeatPlayer::Start(
	CBytebeatEngine* engine,
	int sampleRate)
{
	if (m_running)
		return false;

	m_engine = engine;
	m_sampleRate = sampleRate;
	m_t = 0;

	m_running = true;

	m_thread = CreateThread(
		NULL,
		0,
		AudioThreadProc,
		this,
		0,
		NULL);

	return m_thread != NULL;
}

bool CBytebeatPlayer::StartSigned(
	CBytebeatEngine* engine, 
	int sampleRate)
{
	if (m_running)
		return false;

	m_engine = engine;
	m_sampleRate = sampleRate;
	m_t = 0;

	m_running = true;

	m_thread = CreateThread(
		NULL,
		0,
		AudioSignedThreadProc,
		this,
		0,
		NULL);

	return m_thread != NULL;
}

void CBytebeatPlayer::Stop()
{
	m_running = false;

	if (m_thread)
	{
		WaitForSingleObject(
			m_thread,
			2000);

		CloseHandle(m_thread);
		m_thread = NULL;
	}
}

bool CBytebeatPlayer::IsPlaying() const
{
	return m_running;
}

DWORD WINAPI CBytebeatPlayer::AudioThreadProc(
	LPVOID param)
{
	CBytebeatPlayer* self =
		(CBytebeatPlayer*)param;

	self->AudioLoop();

	return 0;
}

DWORD WINAPI CBytebeatPlayer::AudioSignedThreadProc(
	LPVOID param)
{
	CBytebeatPlayer* self =
		(CBytebeatPlayer*)param;

	self->AudioLoopSigned();

	return 0;
}

void CBytebeatPlayer::AudioLoop()
{
	const int bufferSize = 4096;

	uint8_t buffer[bufferSize];

	while (m_running)
	{
		for (int i = 0; i < bufferSize; i++)
		{
			uint8_t sample = 0;

			if (m_engine &&
				!m_engine->HasParseError())
			{
				m_engine->Sample(m_t, sample);
			}

			buffer[i] = sample;

			m_t++;
		}

		// Play buffer using waveOut (simple blocking)
		WAVEHDR hdr = { 0 };

		hdr.lpData = (LPSTR)buffer;
		hdr.dwBufferLength = bufferSize;

		HWAVEOUT hWave;

		WAVEFORMATEX format;
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = 1;
		format.nSamplesPerSec = m_sampleRate;
		format.wBitsPerSample = 8;
		format.nBlockAlign = 1;
		format.nAvgBytesPerSec = m_sampleRate;
		format.cbSize = 0;

		waveOutOpen(
			&hWave,
			WAVE_MAPPER,
			&format,
			0,
			0,
			CALLBACK_NULL);

		waveOutPrepareHeader(
			hWave,
			&hdr,
			sizeof(hdr));

		waveOutWrite(
			hWave,
			&hdr,
			sizeof(hdr));

		// wait for playback
		while (!(hdr.dwFlags & WHDR_DONE))
		{
			Sleep(1);
		}

		waveOutUnprepareHeader(
			hWave,
			&hdr,
			sizeof(hdr));

		waveOutClose(hWave);
	}
}

void CBytebeatPlayer::AudioLoopSigned()
{
	const int bufferSize = 4096;
	int8_t buffer[bufferSize];
	while (m_running)
	{
		for (int i = 0; i < bufferSize; i++)
		{
			int8_t sample = 0;
			if (m_engine &&
				!m_engine->HasParseError())
			{
				m_engine->SampleSigned(m_t, sample);
			}
			buffer[i] = sample;
			m_t++;
		}
		// Play buffer using waveOut (simple blocking)
		WAVEHDR hdr = { 0 };
		hdr.lpData = (LPSTR)buffer;
		hdr.dwBufferLength = bufferSize;
		HWAVEOUT hWave;
		WAVEFORMATEX format;
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = 1;
		format.nSamplesPerSec = m_sampleRate;
		format.wBitsPerSample = 8;
		format.nBlockAlign = 1;
		format.nAvgBytesPerSec = m_sampleRate;
		format.cbSize = 0;
		waveOutOpen(
			&hWave,
			WAVE_MAPPER,
			&format,
			0,
			0,
			CALLBACK_NULL);
		waveOutPrepareHeader(
			hWave,
			&hdr,
			sizeof(hdr));
		waveOutWrite(
			hWave,
			&hdr,
			sizeof(hdr));
		// wait for playback
		while (!(hdr.dwFlags & WHDR_DONE))
		{
			Sleep(1);
		}
		waveOutUnprepareHeader(
			hWave,
			&hdr,
			sizeof(hdr));
		waveOutClose(hWave);
	}
}
