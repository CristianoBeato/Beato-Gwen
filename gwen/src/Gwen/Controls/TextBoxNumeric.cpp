/*
========================================================================== =
GWEN

Copyright(c) 2010 Facepunch Studios
Copyright(c) 2017 - 2018 Cristiano Beato

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
========================================================================== =
*/

#include "precompiled.h"
#pragma hdrstop

#include "Gwen/Gwen.h"
#include "Gwen/Controls/TextBox.h"
#include "Gwen/Utility.h"
#include "Gwen/Platforms/Platform.h"


using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( TextBoxNumeric )
{
	SetText( L"0" );
}

bool TextBoxNumeric::IsTextAllowed( const Gwen::UnicodeString & str, int iPos )
{
	const UnicodeString & strString = GetText().GetUnicode();

	if ( str.length() == 0 )
	{ return true; }

	for ( size_t i = 0; i < str.length(); i++ )
	{
		if ( str[i] == L'-' )
		{
			// Has to be at the start
			if ( i != 0 || iPos != 0 )
			{ return false; }

			// Can only be one
			if ( std::count( strString.begin(), strString.end(), L'-' ) > 0 )
			{ return false; }

			continue;
		}

		if ( str[i] == L'0' ) { continue; }

		if ( str[i] == L'1' ) { continue; }

		if ( str[i] == L'2' ) { continue; }

		if ( str[i] == L'3' ) { continue; }

		if ( str[i] == L'4' ) { continue; }

		if ( str[i] == L'5' ) { continue; }

		if ( str[i] == L'6' ) { continue; }

		if ( str[i] == L'7' ) { continue; }

		if ( str[i] == L'8' ) { continue; }

		if ( str[i] == L'9' ) { continue; }

		if ( str[i] == L'.' )
		{
			// Already a fullstop
			if ( std::count( strString.begin(), strString.end(), L'.' ) > 0 )
			{ return false; }

			continue;
		}

		return false;
	}

	return true;
}

float TextBoxNumeric::GetFloatFromText()
{
	double temp = Gwen::Utility::Strings::To::Float( GetText().GetUnicode() );
	return temp;
}