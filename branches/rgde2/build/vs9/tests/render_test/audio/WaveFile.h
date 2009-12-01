#pragma once

namespace audio
{
// The Wavefile class implements readim PCM data from a .wav file.
// This implementation is based on the DirectSound SDK samples,
// for more info download the DirectX sdk at http://www.msdn.com/directx
class wave_file
{
    public:
        WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
        HMMIO         m_hmmio;       // MM I/O handle for the WAVE
        MMCKINFO      m_ck;          // Multimedia RIFF chunk
        MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
        DWORD         m_dwSize;      // The size of the wave file
        MMIOINFO      m_mmioinfoOut;
        BOOL          m_bIsReadingFromMemory;
        BYTE*         m_pbData;
        BYTE*         m_pbDataCur;
        ULONG         m_ulDataSize;

        wave_file();
        ~wave_file();

        bool open(const char* strFileName, WAVEFORMATEX* pwfx);

        void close();

        bool read(BYTE* pBuffer, DWORD dwOffset, DWORD dwSizeToRead, DWORD* pdwSizeRead);

        DWORD   get_size();
        HRESULT reset_file();
        WAVEFORMATEX* get_format()  {  return m_pwfx;  };


    protected:
        HRESULT read_mmio();
};
}