/*
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
*/

#ifndef GWEN_TEXTOBJECT_H
#define GWEN_TEXTOBJECT_H

#include "Gwen/Utility.h"

namespace Gwen
{
	/*
		TextObjects can be either a UnicodeString or a String
		Just makes things easier instead of having a function taking both.
	*/
	class TextObject
	{
		public:

			TextObject(void) {}

			TextObject( const Gwen::String & text )
			{
				m_String = text;
				m_Unicode = Gwen::Utility::StringToUnicode( m_String );
			}

			TextObject( const char* text )
			{
				m_String = text;
				m_Unicode = Gwen::Utility::StringToUnicode( m_String );
			}

			TextObject( const wchar_t* text )
			{
				m_Unicode = text;
				m_String = Gwen::Utility::UnicodeToString( m_Unicode );
			}

			TextObject( const Gwen::UnicodeString & unicode )
			{
				*this = unicode;
			}

			operator const Gwen::String & () { return m_String; }
			operator const Gwen::UnicodeString & () { return m_Unicode; }

			void operator = ( const char* str )
			{
				m_String = str;
				m_Unicode = Gwen::Utility::StringToUnicode( m_String );
			}

			void operator = ( const Gwen::String & str )
			{
				m_String = str;
				m_Unicode = Gwen::Utility::StringToUnicode( m_String );
			}

			void operator = ( const Gwen::UnicodeString & unicodeStr )
			{
				m_Unicode = unicodeStr;
				m_String = Gwen::Utility::UnicodeToString( m_Unicode );
			}

			bool operator == ( const TextObject & to ) const
			{
				return m_Unicode == to.m_Unicode;
			}

			const Gwen::String & Get() const
			{
				return m_String;
			}

			const char* c_str() const
			{
				return m_String.c_str();
			}

			const Gwen::UnicodeString & GetUnicode() const
			{
				return m_Unicode;
			}

			int length() const { return m_Unicode.length(); }

			Gwen::UnicodeString		m_Unicode;
			Gwen::String			m_String;
	};
}
#endif
