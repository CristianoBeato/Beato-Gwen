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

#ifndef GWEN_INPUTHANDLER_H
#define GWEN_INPUTHANDLER_H

#include <queue>
#include "Gwen/Gwen.h"

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}

	namespace Key
	{
		const unsigned char Invalid = 0;
		const unsigned char Return = 1;
		const unsigned char Backspace = 2;
		const unsigned char Delete = 3;
		const unsigned char Left = 4;
		const unsigned char Right = 5;
		const unsigned char Shift = 6;
		const unsigned char Tab = 7;
		const unsigned char Space = 8;
		const unsigned char Home = 9;
		const unsigned char End = 10;
		const unsigned char Control = 11;
		const unsigned char Up = 12;
		const unsigned char Down = 13;
		const unsigned char Escape = 14;
		const unsigned char Alt = 15;

		const unsigned char Count = 16;
	}

	namespace Input
	{
		namespace Message
		{
			enum
			{
				Copy,
				Paste,
				Cut,
				Undo,
				Redo,
				SelectAll
			};
		};



		// For use in panels
		bool GWEN_EXPORT IsKeyDown( int iKey );
		bool GWEN_EXPORT IsLeftMouseDown();
		bool GWEN_EXPORT IsRightMouseDown();
		Gwen::Point GWEN_EXPORT GetMousePosition();

		inline bool IsShiftDown() { return IsKeyDown( Gwen::Key::Shift ); }
		inline bool IsControlDown() { return IsKeyDown( Gwen::Key::Control ); }

		// Does copy, paste etc
		bool GWEN_EXPORT DoSpecialKeys( Controls::Base* pCanvas, Gwen::UnicodeChar chr );
		bool GWEN_EXPORT HandleAccelerator( Controls::Base* pCanvas, Gwen::UnicodeChar chr );

		// Send input to canvas for study
		void GWEN_EXPORT OnMouseMoved( Controls::Base* pCanvas, int x, int y, int deltaX, int deltaY );
		bool GWEN_EXPORT OnMouseClicked( Controls::Base* pCanvas, int iButton, bool bDown );
		bool GWEN_EXPORT OnKeyEvent( Controls::Base* pCanvas, int iKey, bool bDown );
		void GWEN_EXPORT OnCanvasThink( Controls::Base* pControl );


	};
}
#endif
