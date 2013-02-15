#ifndef PEGAS_TYPES_H
#define PEGAS_TYPES_H
#pragma once

namespace Pegas
{
	typedef int CURCOORD;
	typedef int RESOURCEID;
	typedef unsigned long FLAGSET;
	
	typedef int FONTSIZE;
	typedef unsigned int RGBCOLOR;

#ifdef _UNICODE
	
	typedef std::wstring String;
	typedef wchar_t tchar;

#define _text(str) L##str

#else
	
	typedef std::string String;
	typedef char tchar;

#define _text(str) str

#endif

	typedef int ANIMATIONID;
	typedef int MILLISECONDS;

	typedef signed long     int32;
	typedef signed short    int16;
	typedef signed char     int8;
	typedef unsigned long   uint32;
	typedef unsigned short  uint16;
	typedef unsigned char   uint8;

	typedef __int64   int64;
	typedef unsigned __int64  uint64;

	extern const int32	MIN_INT32;
	extern const int32	MAX_INT32;
	extern const int16	MIN_INT16;
	extern const int16	MAX_INT16;
	extern const int8	MIN_INT8;
	extern const int8	MAX_INT8;
	extern const uint32	MIN_UINT32;
	extern const uint32	MAX_UINT32;
	extern const uint16	MIN_UINT16;
	extern const uint16	MAX_UINT16;
	extern const uint8	MIN_UINT8;
	extern const uint8	MAX_UINT8;

	extern const float	MIN_REAL32;
	extern const float	MAX_REAL32;
	extern const float	TINY_REAL32;
}

#endif //PEGAS_TYPES_H