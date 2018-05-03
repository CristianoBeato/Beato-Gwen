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

#include "Gwen/Gwen.h"
#include <stdio.h>
#include <stdarg.h>

namespace Gwen
{
	// Globals
	GWEN_EXPORT Controls::Base* HoveredControl = NULL;
	GWEN_EXPORT Controls::Base* KeyboardFocus = NULL;
	GWEN_EXPORT Controls::Base* MouseFocus = NULL;

	namespace Debug
	{
		void Msg( const char* str, ... )
		{
			char strOut[1024];
			va_list s;
			va_start( s, str );
			vsnprintf( strOut, sizeof( strOut ), str, s );
			va_end( s );
			GwenUtil_OutputDebugCharString( strOut );
		}
#ifdef UNICODE
		void Msg( const wchar_t* str, ... )
		{
			wchar_t strOut[1024];
			va_list s;
			va_start( s, str );
			vswprintf( strOut, sizeof( strOut ), str, s );
			va_end( s );
			GwenUtil_OutputDebugWideString( strOut );
		}
#endif
		void AssertCheck( bool b, const char* strMsg )
		{
			if ( b ) { return; }

			Msg( "Assert: %s\n", strMsg );
#ifdef _WIN32
			MessageBoxA( NULL, strMsg, "Assert", MB_ICONEXCLAMATION | MB_OK );
			__debugbreak();
#endif
		}
	}

}
