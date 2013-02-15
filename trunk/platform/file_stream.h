#pragma once

namespace Pegas
{
	/*
	class FileStream: public ISerializer
	{
	public:
		~FileStream();

		enum eMode
		{
			k_modeRead = 1,
			k_modeWrite = 2,
			k_modeBinary = 4,
			k_modeAtTheEnd = 8,
			k_modeAppend = 16,
			k_modeTruncate = 32
		};

		bool open(const String& file, int32 mode = k_modeRead|k_modeWrite|k_modeBinary);
		void close();

		virtual ISerializer& operator>>(int8& data);
		virtual ISerializer& operator>>(uint8& data);
		virtual ISerializer& operator>>(int16& data);
		virtual ISerializer& operator>>(uint16& data);
		virtual ISerializer& operator>>(int32& data);
		virtual ISerializer& operator>>(uint32& data);
		virtual ISerializer& operator>>(float& data);
		virtual ISerializer& operator>>(bool& data);
		virtual ISerializer& operator>>(String& data);

		virtual ISerializer& operator<<(int8 data);
		virtual ISerializer& operator<<(uint8 data);
		virtual ISerializer& operator<<(int16 data);
		virtual ISerializer& operator<<(uint16 data);
		virtual ISerializer& operator<<(int32 data);
		virtual ISerializer& operator<<(uint32 data);
		virtual ISerializer& operator<<(float data);
		virtual ISerializer& operator<<(bool data);
		virtual ISerializer& operator<<(String& data);

	private:

#ifdef _UNICODE
		std::wfstream	m_stream;
#else
		std::fstream	m_stream;
#endif

	};*/
}