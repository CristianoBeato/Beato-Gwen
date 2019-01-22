/*
===========================================================================
GWEN

Copyright (c) 2010 Facepunch Studios
Copyright (c) 2017-2019 Cristiano Beato

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

#ifndef GWEN_UTILITY_H
#define GWEN_UTILITY_H

#include <sstream>
#include <vector>
#include "Gwen/Structures.h"

namespace Gwen
{
	class TextObject;

	namespace Utility
	{
		template <typename T>
		const T & Max( const T & x, const T & y )
		{
			if ( y < x ) { return x; }

			return y;
		}

		template <typename T>
		const T & Min( const T & x, const T & y )
		{
			if ( y > x ) { return x; }

			return y;
		}

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

		inline String UnicodeToString( const UnicodeString & strIn )
		{
			if ( !strIn.length() ) { return ""; }

			String temp( strIn.length(), ( char ) 0 );
			std::use_facet< std::ctype<wchar_t> > ( std::locale() ). \
			narrow( &strIn[0], &strIn[0] + strIn.length(), ' ', &temp[0] );
			return temp;
		}

		inline UnicodeString StringToUnicode( const String & strIn )
		{
			if ( !strIn.length() ) { return L""; }

			UnicodeString temp( strIn.length(), ( wchar_t ) 0 );
			std::use_facet< std::ctype<wchar_t> > ( std::locale() ). \
			widen( &strIn[0], &strIn[0] + strIn.length(), &temp[0] );
			return temp;
		}

		template<typename T> void Replace( T & str, const T & strFind, const T & strReplace )
		{
			size_t pos = 0;

			while ( ( pos = str.find( strFind, pos ) ) != T::npos )
			{
				str.replace( pos, strFind.length(), strReplace );
				pos += strReplace.length();
			}
		}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

		template <class T>
		String ToString( const T & object )
		{
			std::ostringstream os;
			os << object;
			return os.str();
		}

		inline Gwen::Rect ClampRectToRect( Gwen::Rect inside, Gwen::Rect outside, bool clampSize = false )
		{
			if ( inside.x < outside.x )
			{ inside.x = outside.x; }

			if ( inside.y  < outside.y )
			{ inside.y = outside.y; }

			if ( inside.x + inside.w > outside.x + outside.w )
			{
				if ( clampSize )
				{ inside.w = outside.w; }
				else
				{ inside.x = outside.x + outside.w - inside.w; }
			}

			if ( inside.y + inside.h > outside.y + outside.h )
			{
				if ( clampSize )
				{ inside.h = outside.h; }
				else
				{ inside.y = outside.w + outside.h - inside.h; }
			}

			return inside;
		}

		GWEN_EXPORT UnicodeString Format( const wchar_t* fmt, ... );

		namespace Strings
		{
			typedef std::vector<Gwen::String> List;
			typedef std::vector<Gwen::UnicodeString> UnicodeList;

			GWEN_EXPORT void Split( const Gwen::String & str, const Gwen::String & seperator, Strings::List & outbits, bool bLeaveSeperators = false );
			GWEN_EXPORT void Split( const Gwen::UnicodeString & str, const Gwen::UnicodeString & seperator, Strings::UnicodeList & outbits, bool bLeaveSeperators = false );
			GWEN_EXPORT bool Wildcard( const Gwen::TextObject & strWildcard, const Gwen::TextObject & strHaystack );

			GWEN_EXPORT void ToUpper( Gwen::UnicodeString & str );
			GWEN_EXPORT void Strip( Gwen::UnicodeString & str, const Gwen::UnicodeString & chars );

			template <typename T>
			T TrimLeft( const T & str, const T & strChars )
			{
				T outstr = str;
				outstr.erase( 0, outstr.find_first_not_of( strChars ) );
				return outstr;
			}

			namespace To
			{
				GWEN_EXPORT bool Bool( const Gwen::String & str );
				GWEN_EXPORT int Int( const Gwen::String & str );
				GWEN_EXPORT float Float( const Gwen::String & str );
				GWEN_EXPORT float Float( const Gwen::UnicodeString & str );
				GWEN_EXPORT bool Floats( const Gwen::String & str, float* f, size_t iCount );
			}
		}
	}



}
#endif
