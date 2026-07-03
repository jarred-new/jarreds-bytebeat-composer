#include "stdafx.h"
#include "WavExporter.h"
#include "BytebeatEngine.h"
#include "Writer16_32.h"
#include "resource.h"
#include <algorithm>

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
	bool isCEngine,
	int isSigned)
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
			if (isSigned == 0)
			{
				uint8_t sample;
				if (!engine.Sample(t, sample))
					sample = 0;

				putchar(sample);
			} else if (isSigned == 1) {
				int8_t sample;
				if (!engine.SampleSigned(t, sample))
					sample = 0;

				putchar(sample);
			}
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

		// WAV parameters
		const uint16_t numChannels = 1;
		const uint16_t bitsPerSample = 8; // original code writes 1 byte per sample
		const uint16_t bytesPerSample = bitsPerSample / 8;
		const uint32_t totalSamples = static_cast<uint32_t>(sampleRate) * static_cast<uint32_t>(seconds);
		const uint32_t dataSize = totalSamples * bytesPerSample;
		const uint32_t byteRate = sampleRate * numChannels * bytesPerSample;
		const uint16_t blockAlign = numChannels * bytesPerSample;

		// Write RIFF header (little-endian)
		struct RiffHeader
		{
			char riff[4];
			uint32_t chunkSize;
			char wave[4];
			char fmt[4];
			uint32_t subchunk1Size;
			uint16_t audioFormat;
			uint16_t numChannels;
			uint32_t sampleRate;
			uint32_t byteRate;
			uint16_t blockAlign;
			uint16_t bitsPerSample;
			char data[4];
			uint32_t dataSize;
		} hdr;

		memcpy(hdr.riff, "RIFF", 4);
		hdr.chunkSize = 36 + dataSize;
		memcpy(hdr.wave, "WAVE", 4);
		memcpy(hdr.fmt, "fmt ", 4);
		hdr.subchunk1Size = 16; // PCM
		hdr.audioFormat = 1; // PCM
		hdr.numChannels = numChannels;
		hdr.sampleRate = static_cast<uint32_t>(sampleRate);
		hdr.byteRate = byteRate;
		hdr.blockAlign = blockAlign;
		hdr.bitsPerSample = bitsPerSample;
		memcpy(hdr.data, "data", 4);
		hdr.dataSize = dataSize;

		file.Write(&hdr, sizeof(hdr));

		// ----------------------------
		// AUDIO LOOP (buffered)
		// ----------------------------

		const size_t BUFFER_SAMPLES = 4096;
		std::vector<char> buffer;
		buffer.resize(BUFFER_SAMPLES * bytesPerSample);

		uint32_t samplesWritten = 0;
		while (samplesWritten < totalSamples)
		{
			uint32_t chunk = std::min<uint32_t>(BUFFER_SAMPLES, totalSamples - samplesWritten);

			for (uint32_t i = 0; i < chunk; ++i)
			{
				uint32_t t = samplesWritten + i;

				if (isSigned == 0)
				{
					uint8_t sample = 0;
					if (!engine.HasParseError())
					{
						engine.Sample(t, sample);
					}
					// 8-bit WAV is typically unsigned; keep user's value as-is
					buffer[i] = static_cast<char>(sample);
				}
				else // isSigned == 1
				{
					int8_t sample = 0;
					if (!engine.HasParseError())
					{
						engine.SampleSigned(t, sample);
					}
					// convert signed int8_t to stored byte preserving bit-pattern
					buffer[i] = *reinterpret_cast<char*>(&sample);
				}
			}

			file.Write(buffer.data(), chunk * bytesPerSample);
			samplesWritten += chunk;
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