#pragma once

namespace Pegas
{
	
		class File: public ISerializer
		{
		public:

			// data Types & Constants...
			enum FILE_ACCESS
			{
				READ_ONLY,
				WRITE_ONLY,
				READ_WRITE,
				APPEND_ONLY,
				READ_AND_APPEND
			};

			// Creators...
			File();
			File(const String& FileName, uint32 AccessRights=READ_WRITE);
			~File();

			// file creation
			bool open();
			bool open(const String& FileName, uint32 AccessRights=READ_WRITE);
			bool create();
			bool create(const String& FileName);
			void close();

			// push/pull operators...
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

			// writing functions
			bool write(const void *Src, uint32 BytesToWrite);
			bool write(const String& string, uint32* pBytesWritten=0);
			bool write(int8 Value);
			bool write(uint8 Value);
			bool write(int16 Value);
			bool write(uint16 Value);
			bool write(int32 Value);
			bool write(uint32 Value);
			bool write(float Value);
			bool write(bool Value);

			// reading functions
			bool read(void *Dest, uint32 BytesToRead);
			bool read(String& string);
			bool read(int8& Value);
			bool read(uint8& Value);
			bool read(int16& Value);
			bool read(uint16& Value);
			bool read(int32& Value);
			bool read(uint32& Value);
			bool read(float& Value);
			bool read(bool& Value);

			// file position
			uint32 position();
			bool setPosition(uint32 NewPos);
			bool seek(long Offset);

			// accessors...
			bool isFileOpen();
			const String& name();
			uint32 size();

		private:

			// data...
			uint32 m_fileAccess;
			void *m_fileHandle;
			String m_fileName;

			// Nonexistant Functions...
			File( const File&  Src);
			File& operator=( const File& Src);
			bool operator == ( const File&  Src) const;
		};

		//- Inline Functions ---------------------------------------------------------------------

		//. Creators .............................................................................
		inline File::File()
		:m_fileHandle(0)
		{
			m_fileName.erase();
		}

		inline File::File(const String& FileName, uint32 AccessRights)
		:m_fileHandle(0)
		,m_fileName(FileName)
		,m_fileAccess(AccessRights)
		{
		}

		inline File::~File()
		{
			close();
		}

		//. Operators ............................................................................

		inline ISerializer& File::operator>>(int8& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(uint8& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(int16& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(uint16& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(int32& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(uint32& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(float& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(bool& data)
		{
			read((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator>>(String& data)
		{
			read(data);
			return(*this);
		}

		inline ISerializer& File::operator<<(int8 data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(uint8 data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(int16 data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(uint16 data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(int32 data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(uint32 data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(float data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(bool data)
		{
			write((void*)&data, sizeof(data));
			return(*this);
		}

		inline ISerializer& File::operator<<(String& data)
		{
			write(data);
			return(*this);
		}


		//. Accessors ............................................................................

		inline bool File::isFileOpen()
		{
			return (m_fileHandle ? true : false);
		}

		inline const String& File::name()
		{
			return (m_fileName);
		}	
} 
