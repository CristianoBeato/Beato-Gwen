/*
===========================================================================
GWEN

Copyright (c) 2010 Facepunch Studios
Copyright (c) 2017-2018 Cristiano Beato

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include <cstdio>
#include "Gwen/Utility.h"

using namespace Gwen;

#ifdef _MSC_VER
#pragma warning(disable:4267)// conversion from 'size_t' to 'int', possible loss of data
#endif

#ifdef __MINGW32__
#undef vswprintf
#define vswprintf _vsnwprintf
#endif

#ifdef _MSC_VER
#define GWEN_FNULL "NUL"
#define va_copy(d,s) ((d) = (s))
#else
#define GWEN_FNULL "/dev/null"
#endif

UnicodeString Gwen::Utility::Format( const wchar_t* fmt, ... )
{
	va_list s;
	int len = 0;
	
	va_start( s, fmt );
	
	// Determine the length of the resulting string, this method is much faster
	// than looping and reallocating a bigger buffer size.
	{	
		FILE* fnull = fopen( GWEN_FNULL, "wb" );
		va_list c;
		va_copy( c, s );
		len = vfwprintf( fnull, fmt, c );
		va_end( c );
		fclose( fnull );
	} 
	
	UnicodeString strOut;
	
	if (len > 0)
	{
		strOut.resize( len + 1 );
		va_list c;
		va_copy( c, s );
		len = vswprintf( &strOut[0], strOut.size(), fmt, c );
		va_end( c );
		strOut.resize( len );
	}
	
	va_end( s );
	
	return strOut;
}



void Gwen::Utility::Strings::Split( const Gwen::String & str, const Gwen::String & seperator, Strings::List & outbits, bool bLeave )
{
	int iOffset = 0;
	int iLength = str.length();
	int iSepLen = seperator.length();
	size_t i = str.find( seperator, 0 );

	while ( i != std::string::npos )
	{
		outbits.push_back( str.substr( iOffset, i - iOffset ) );
		iOffset = i + iSepLen;
		i = str.find( seperator, iOffset );

		if ( bLeave ) { iOffset -= iSepLen; }
	}

	outbits.push_back( str.substr( iOffset, iLength - iOffset ) );
}

void Gwen::Utility::Strings::Split( const Gwen::UnicodeString & str, const Gwen::UnicodeString & seperator, Strings::UnicodeList & outbits, bool bLeave )
{
	int iOffset = 0;
	int iLength = str.length();
	int iSepLen = seperator.length();
	size_t i = str.find( seperator, 0 );

	while ( i != std::wstring::npos )
	{
		outbits.push_back( str.substr( iOffset, i - iOffset ) );
		iOffset = i + iSepLen;
		i = str.find( seperator, iOffset );

		if ( bLeave ) { iOffset -= iSepLen; }
	}

	outbits.push_back( str.substr( iOffset, iLength - iOffset ) );
}

int Gwen::Utility::Strings::To::Int( const Gwen::String & str )
{
	if ( str == "" ) { return 0; }

	return atoi( str.c_str() );
}

float Gwen::Utility::Strings::To::Float( const Gwen::String & str )
{
	if ( str == "" ) { return 0.0f; }

	return ( float ) atof( str.c_str() );
}

float Gwen::Utility::Strings::To::Float( const Gwen::UnicodeString & str )
{
	return wcstod( str.c_str(), NULL );
}

bool Gwen::Utility::Strings::To::Bool( const Gwen::String & str )
{
	if ( str.size() == 0 ) { return false; }

	if ( str[0] == 'T' || str[0] == 't' || str[0] == 'y' || str[0] == 'Y' ) { return true; }

	if ( str[0] == 'F' || str[0] == 'f' || str[0] == 'n' || str[0] == 'N' ) { return false; }

	if ( str[0] == '0' ) { return false; }

	return true;
}

bool Gwen::Utility::Strings::To::Floats( const Gwen::String & str, float* f, size_t iCount )
{
	Strings::List lst;
	Strings::Split( str, " ", lst );

	if ( lst.size() != iCount ) { return false; }

	for ( size_t i = 0; i < iCount; i++ )
	{
		f[i] = Strings::To::Float( lst[i] );
	}

	return true;
}


bool Gwen::Utility::Strings::Wildcard( const TextObject & strWildcard, const TextObject & strHaystack )
{
	const UnicodeString & W = strWildcard.GetUnicode();
	const UnicodeString & H = strHaystack.GetUnicode();

	if ( strWildcard == "*" ) { return true; }

	int iPos = W.find( L"*", 0 );

	if ( iPos == UnicodeString::npos ) { return strWildcard == strHaystack; }

	// First half matches
	if ( iPos > 0 && W.substr( 0, iPos ) != H.substr( 0, iPos ) )
	{ return false; }

	// Second half matches
	if ( iPos != W.length() - 1 )
	{
		UnicodeString strEnd = W.substr( iPos + 1, W.length() );

		if ( strEnd != H.substr( H.length() - strEnd.length(), H.length() ) )
		{ return false; }
	}

	return true;
}



void Gwen::Utility::Strings::ToUpper( Gwen::UnicodeString & str )
{
	transform( str.begin(), str.end(), str.begin(), towupper );
}

void Gwen::Utility::Strings::Strip( Gwen::UnicodeString & str, const Gwen::UnicodeString & chars )
{
	Gwen::UnicodeString Source = str;
	str = L"";

	for ( unsigned int i = 0; i < Source.length(); i++ )
	{
		if ( chars.find( Source[i] ) != Gwen::UnicodeString::npos )
		{ continue; }

		str += Source[i];
	}
}



