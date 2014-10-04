#pragma once

#include "types.h"

#include <stdio.h> 
#include <stdarg.h> 
#include <tchar.h> 

#ifndef STD_STRING_H
#define STD_STRING_H
#pragma warning(disable : 4786)
#include <string>
#endif

//	Name Space declaration
namespace Pegas
{
	template<typename _Char>
	class TString: public std::basic_string<_Char, std::char_traits<_Char> >
	{
	public:
		typedef std::basic_string<_Char, std::char_traits<_Char> > TEXT_BASE_CLASS;
		
		// Creators...
		inline TString(){};
		TString(const TString<_Char>& rhs);
		TString(const _Char* text, ...);
		explicit TString(const TEXT_BASE_CLASS& rhs);
		explicit TString(_Char c);
		~TString(){};

		// cast the string directly to a _Char pointer
		inline operator const _Char*()const;

		// copy data from various source types
		TString<_Char>& operator=(const TString<_Char>& rhs);
		TString<_Char>& operator=(const TEXT_BASE_CLASS& rhs);
		TString<_Char>& operator=(const _Char *s);
		TString<_Char>& operator=(_Char c);

		// append data from various sources
		TString<_Char>& operator+=(const TString<_Char>& rhs);
		TString<_Char>& operator+=(const TEXT_BASE_CLASS& rhs);
		TString<_Char>& operator+=(const _Char *s); 
		TString<_Char>& operator+=(_Char c);

		// test equality and inequality (case sensitive)
		bool operator==(const TString<_Char>& rhs);
		bool operator!=(const TString<_Char>& rhs);

		TString<_Char>& trim(_Char c = ' ');
		TString<_Char>& trimStart(_Char c = ' ');
		TString<_Char>& trimEnd(_Char c = ' ');

		//specialization

		// test without case sensitivity
		bool compareWithoutCase(const TString<_Char>& rhs);

		// operator that allows TString<_Char> to be used as an STL map key value
		bool operator<(const TString<_Char>& rhs);

		// format the string from optional parameters
		void __cdecl format(const _Char* text, ...);
		
		//additional functions
		TString<_Char>& toLower();
		TString<_Char>& toUpper();
	};

	//- Inline Functions ---------------------------------------------------------------------

	//. Creators .............................................................................

	template<typename _Char>
	inline TString<_Char>::TString(const TString<_Char>& rhs)
	{
		assign((const TEXT_BASE_CLASS) rhs); 
	}

	template<typename _Char>
	inline TString<_Char>::TString(const TEXT_BASE_CLASS& rhs)
	{
		assign((const TEXT_BASE_CLASS) rhs); 
	}

	template<typename _Char>
	inline TString<_Char>::TString(_Char c)
	{
		assign(1,c); 
	}

	//. Operators ............................................................................
	template<typename _Char>
	inline bool TString<_Char>::operator==(const TString<_Char>& rhs)
	{
		return (compare(rhs) == 0);
	}

	template<typename _Char>
	inline bool TString<_Char>::operator!=(const TString<_Char>& rhs)
	{
		return (compare(rhs) != 0);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(const TString<_Char>& rhs) 
	{
		assign((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(const TEXT_BASE_CLASS& rhs) 
	{
		assign((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(const _Char *s)
	{
		assign(s); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(_Char c)
	{
		assign(1,c); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(const TString<_Char>& rhs)
	{
		append((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(const TEXT_BASE_CLASS& rhs)
	{
		append((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(const _Char *s)
	{
		append(s); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(_Char c)
	{
		append(1,c); 
		return (*this);
	}


	//. Helper Functions .....................................................................


	template<typename _Char>
	inline TString<_Char> operator+(const TString<_Char>& lhs, const TString<_Char>& rhs)
	{
		TString<_Char> NewText(lhs);
		NewText+=rhs;
		return(NewText);
	}

	template<typename _Char>
	inline TString<_Char>::operator const _Char*()const
	{
		return (c_str());
	}

	/*************************************************************************************
		additional operations
	**************************************************************************************/
	template<typename _Char>
	inline TString<_Char>& TString<_Char>::trim(_Char c)
	{
		return trimStart(c).trimEnd(c);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::trimStart(_Char c)
	{
		size_t found_pos = find_first_not_of(c);
		if(found_pos != npos)
		{
			TEXT_BASE_CLASS trimmedStr(begin() + found_pos, end());
			assign(trimmedStr);
		}

		return *this;
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::trimEnd(_Char c)
	{
		size_t new_length = length();
		for(size_t i = length() - 1; i >= 0; --i)
		{
			if((at(i) != c) && (at(i) != _Char(0)))
				break;
			--new_length;
		}

		if(new_length != length())
		{
			resize(new_length);
		}

		return *this;
	}

	/*************************************************************************************************
		ANSIString
	*************************************************************************************************/

	template<>
	inline TString<char>::TString(const char* text, ...)
	{
		va_list args;
		int len;
		char* buffer;

		va_start( args, text );
		// _vscprintf doesn't count  terminating '\0'
		len = _vscprintf( text, args ) + 1;
		buffer = new char[len];
		
		vsprintf_s( buffer, len, text, args );
		assign(buffer);
		
		delete[] buffer;
	}

	// test without case sensitivity
	template<>
	inline bool TString<char>::compareWithoutCase(const TString<char>& rhs)
	{
		return(_stricmp(c_str(), rhs.c_str()) == 0);
	}

	// operator that allows TString<_Char> to be used as an STL map key value
	template<>
	inline bool TString<char>::operator<(const TString<char>& rhs)
	{
		return(strcmp(c_str(), rhs.c_str()) < 0);
	}

	// format the string from optional parameters
	template<>
	inline void __cdecl TString<char>::format(const char* text, ...)
	{
		va_list args;
		int len;
		char* buffer;

		va_start( args, text );
		// _vscprintf doesn't count  terminating '\0'
		len = _vscprintf( text, args ) + 1;
		buffer = new char[len];
		
		vsprintf_s( buffer, len, text, args );
		assign(buffer);
		
		delete[] buffer;
	}

	//additional functions
	template<>
	inline TString<char>& TString<char>::toLower()
	{
		if(at(length() - 1) != '\0')
		{
			append(1, '\0');
		}

		_strlwr_s(&(*begin()), length());

		return *this;
	}

	template<>
	inline TString<char>& TString<char>::toUpper()
	{
		if(at(length() - 1) != '\0')
		{
			append(1, '\0');
		}

		_strupr_s(&(*begin()), length());							

		return *this;
	}

	/****************************************************************************************
		WideString
	*****************************************************************************************/
	template<>
	inline TString<wchar_t>::TString(const wchar_t* text, ...)
	{
		va_list args;
		int len;
		wchar_t* buffer;

		va_start( args, text );

		// _vscprintf doesn't count  terminating '\0'
		len = _vscwprintf( text, args ) + 1;
		
		buffer = new wchar_t[len];
					
		vswprintf_s( buffer, len, text, args );
		
		assign(buffer);
		
		delete[] buffer;
	}

	// test without case sensitivity
	template<>
	inline bool TString<wchar_t>::compareWithoutCase(const TString<wchar_t>& rhs)
	{
		return(_wcsicmp(c_str(), rhs.c_str()) == 0);
	}

	// operator that allows TString<_Char> to be used as an STL map key value
	template<>
	inline bool TString<wchar_t>::operator<(const TString<wchar_t>& rhs)
	{
		return(wcscmp(c_str(), rhs.c_str()) < 0);
	}

	// format the string from optional parameters
	template<>
	inline void __cdecl TString<wchar_t>::format(const wchar_t* text, ...)
	{
		va_list args;
		int len;
		wchar_t* buffer;

		va_start( args, text );

		// _vscprintf doesn't count  terminating '\0'
		len = _vscwprintf( text, args ) + 1;
		
		buffer = new wchar_t[len];
					
		vswprintf_s( buffer, len, text, args );
		
		assign(buffer);
		
		delete[] buffer;
	}

	//additional functions
	template<>
	inline TString<wchar_t>& TString<wchar_t>::toLower()
	{
		if(at(length() - 1) != L'\0')
		{
			append(1, L'\0');
		}

		_wcslwr_s(&(*begin()), length());

		return *this;
	}

	template<>
	inline TString<wchar_t>& TString<wchar_t>::toUpper()
	{
		if(at(length() - 1) != L'\0')
		{
			append(1, L'\0');
		}

		_wcsupr_s(&(*begin()), length());							

		return *this;
	}

	typedef TString<char> AString;
	typedef TString<wchar_t> WString;
	typedef TString<tchar> String;
} 

