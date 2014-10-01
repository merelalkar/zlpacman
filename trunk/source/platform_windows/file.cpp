#include "platform_windows.h"
using namespace Pegas;

bool File::open()
{
	assert(m_fileName.size() != 0 && "No filename provided");

	return open(m_fileName, m_fileAccess);
}

bool File::open(const String& file_name, uint32 access_rights)
{
	char file_access[3];

	close();

	switch(access_rights)
	{
		case READ_ONLY:
			strcpy(file_access,"rb");
			break;
		case WRITE_ONLY:
			strcpy(file_access,"wb");
			break;
		case APPEND_ONLY:
			strcpy(file_access,"ab");
			break;
		case READ_AND_APPEND:
			strcpy(file_access,"a+b");
			break;

		default:
		case READ_WRITE:
			strcpy(file_access,"r+b");
			break;

	}

	char fileName[256];
	wcstombs(fileName, file_name.c_str(), 256);

	m_fileHandle = (void*)fopen(fileName,file_access);
	
	if(m_fileHandle==0)
	{
		return (false);
	}

	m_fileAccess=access_rights;
	m_fileName=file_name;

	return (true);
}

bool File::create()
{
	return open(m_fileName);
}

bool File::create(const String& file_name)
{
	close();

	char fileName[256];
	wcstombs(fileName, file_name.c_str(), 256);

	m_fileHandle=(void*)fopen(fileName,"w+b");
	
	if(m_fileHandle==0)
	{
		return (false);
	}

	m_fileAccess=READ_WRITE;
	m_fileName=file_name;

	return (true);
}

void File::close()
{
	if (m_fileHandle)
	{
		fflush((FILE*)m_fileHandle);
		fclose((FILE*)m_fileHandle);
		m_fileHandle = 0;
	}
}

bool File::read(void *Dest, uint32 BytesToRead)
{
	unsigned long BytesRead = 0;

	assert(m_fileHandle && "There is no file handle open");

	BytesRead = fread(Dest, 1, BytesToRead, (FILE*)m_fileHandle);

	return (BytesRead == BytesToRead);
}

bool File::write(const void *Src, uint32 BytesToWrite)
{
	unsigned long BytesWritten = 0;

	assert(m_fileHandle && "There is no file handle open");

	if (BytesToWrite && Src)
	{
		BytesWritten = fwrite(Src, 1, BytesToWrite, (FILE*)m_fileHandle);
	}
	return (BytesWritten == BytesToWrite);
}

bool File::write(int32 value)
{
	return(write(&value, sizeof(int32)));
}

bool File::write(float value)
{
	return(write(&value, sizeof(float)));
}

bool File::write(uint32 value)
{
	return(write(&value, sizeof(uint32)));
}

bool File::write(bool value)
{
	return(write(&value, sizeof(bool)));
}


bool File::read(int32& value)
{
	return(read(&value, sizeof(int)));
}

bool File::read(float& value)
{
	return(read(&value, sizeof(float)));
}

bool File::read(uint32& value)
{
	return(read(&value, sizeof(uint32)));
}

bool File::read(bool& value)
{
	return(read(&value, sizeof(bool)));
}


uint32 File::position()
{
	assert(m_fileHandle && "There is no file handle open");

	return (ftell((FILE*)m_fileHandle));
}

bool File::setPosition(uint32 NewPos)
{
	assert(m_fileHandle && "There is no file handle open");

	if(!fseek((FILE*)m_fileHandle, (long)NewPos, SEEK_SET))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool File::seek(long Offset)
{
	assert(m_fileHandle && "There is no file handle open");

	if(!fseek((FILE*)m_fileHandle, (long)Offset, SEEK_CUR))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool File::write(const String& string, uint32* pBytesWritten)
{
	char* name =0;
	int32 length = string.size() + 1;

	if (write(length))
	{
		if (!length || write((void*)string.c_str(), length * sizeof(tchar)))
		{
			if (pBytesWritten)
			{
				*pBytesWritten = length + sizeof(length);
			}
			return true;
		}
	}
	return(false);
}

bool File::read(String& string)
{
	bool result = false;
	tchar* name = 0;
	int32 length = 0;

	if(read(length))
	{
		if (length)
		{
			name = new tchar[length];
			if(read(name, length * sizeof(tchar)))
			{
				string = name;
				delete [](name);
				result = true;
			}
		}
		else
		{
			string.erase();
			result = true;
		}
	}
	return(result);
}

uint32 File::size()
{
	assert(m_fileHandle && "There is no file handle open");

	uint32 CurrentPos = position();

	fseek((FILE*)m_fileHandle, (long)0, SEEK_END);

	uint32 size=position();

	setPosition(CurrentPos);

	return (size);
}

