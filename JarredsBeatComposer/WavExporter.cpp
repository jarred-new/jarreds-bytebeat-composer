#include "stdafx.h"
#include "WavExporter.h"
#include "BytebeatEngine.h"
#include "Writer16_32.h"
#include "resource.h"

#pragma pack(push, 1)
struct WAVHeader
{
	char riff[4];
	uint32_t riffSize;
	char wave[4];

	char fmt[4];
	uint32_t fmtSize;
	uint16_t format;
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;

	char data[4];
	uint32_t dataSize;
};
#pragma pack(pop)

bool CWavExporter::Export(LPCTSTR path,
	CBytebeatEngine& engine,
	int sampleRate,
	int seconds,
	bool isCEngine)
{
	if (isCEngine)
	{
		// BeatC engine (Fast)
		// 1. Force stdout to write to a binary file in Cxxdroid's working directory
		CWriter16_32 writer;

		CString cfile(path);
		std::wstring fileW(cfile);
		std::string fileS(fileW.begin(), fileW.end());

		if (freopen(fileS.c_str(), "wb", stdout) == NULL) return 1;

		int sample_rate = sampleRate;
		int num_samples = sample_rate * seconds;
		int data_size = num_samples; // 1 byte per sample for 8-bit mono

		// 2. RIFF Header (44 bytes total)
		fwrite("RIFF", 1, 4, stdout);
		writer.put32(36 + data_size); // File size - 8
		fwrite("WAVE", 1, 4, stdout);
		fwrite("fmt ", 1, 4, stdout);
		writer.put32(16);             // Subchunk size
		writer.put16(1);              // PCM format
		writer.put16(1);              // Mono
		writer.put32(sample_rate);    // Sample rate
		writer.put32(sample_rate);    // Byte rate
		writer.put16(1);              // Block align
		writer.put16(8);              // Bits per sample

		// 3. Data Chunk
		fwrite("data", 1, 4, stdout);
		writer.put32(data_size);

		// 4. Generate Audio (Sine Wave)
		for (int t = 0; t < num_samples; t++) {
			// Simple 440Hz tone
			uint8_t sample;

			if (!engine.Sample(t, sample))
				sample = 0;

			putchar(sample);
		}

		CString msg;
		msg.Format(_T("Exported WAV:\n%s"), path);
		AfxMessageBox(msg);

		return 0;
	}
	else
	{
		// Carmen (slower)
		// An Engine using MFC's CFile
		// Named after a classic song

		if (!path || sampleRate <= 0 || seconds <= 0)
			return false;

		CFile file;

		if (!file.Open(path,
			CFile::modeCreate |
			CFile::modeWrite |
			CFile::typeBinary))
		{
			AfxMessageBox(_T("Cannot create WAV file"));
			return false;
		}

		uint32_t totalSamples =
			sampleRate * seconds;

		uint32_t dataSize =
			totalSamples;

		WAVHeader header;

		header.sampleRate = sampleRate;
		header.byteRate = sampleRate;
		header.blockAlign = 1;
		header.dataSize = dataSize;
		header.riffSize = 36 + dataSize;

		file.Write(&header, sizeof(header));

		// ----------------------------
		// AUDIO LOOP
		// ----------------------------

		for (uint32_t t = 0; t < totalSamples; t++)
		{
			uint8_t sample = 0;

			if (engine.HasParseError())
			{
				sample = 0;
			}
			else
			{
				engine.Sample(t, sample);
			}

			file.Write(&sample, 1);
		}

		// ----------------------------
		// CLEANUP
		// ----------------------------

		file.Flush();
		file.Close();

		CString msg;
		msg.Format(_T("Exported WAV:\n%s"), path);
		AfxMessageBox(msg);

		return true;
	}
}