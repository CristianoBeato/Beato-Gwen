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

#include <SDL_events.h>

#ifdef _WIN32
#define UCS_STRING "UCS-2"
#else
#define UCS_STRING "UCS-4"
#endif


using namespace Gwen;

Input::SDL2::SDL2()
{
	m_Canvas = NULL;
}

void Gwen::Input::SDL2::Initialize(Gwen::Controls::Canvas * c)
{
	m_Canvas = c;
}

bool Gwen::Input::SDL2::ProcessMouseEvents(SDL_Event Event, Uint32 mWindowID)
{
	if (!m_Canvas)
		return false;

	//If an event was detected for this window
	if (Event.window.windowID == mWindowID)
	{
		switch (Event.type)
		{
		case SDL_MOUSEMOTION:
		{
			SDL_MouseMotionEvent* moEvent = &Event.motion;
			return m_Canvas->InputMouseMoved(moEvent->x, moEvent->y, moEvent->xrel, moEvent->yrel);
		}

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			SDL_MouseButtonEvent bmEvent = Event.button;
			int Button = -1;

			switch (bmEvent.button)
			{
			case SDL_BUTTON_LEFT:
				Button = 0;
				break;

			case SDL_BUTTON_MIDDLE:
				Button = 2;
				break;

			case SDL_BUTTON_RIGHT:
				Button = 1;
				break;

			default:
				return false;
			}
			return m_Canvas->InputMouseButton(Button, bmEvent.state);
		}
		case SDL_MOUSEWHEEL:
		{
			SDL_MouseWheelEvent wmEvent = Event.wheel;
			return m_Canvas->InputMouseWheel(wmEvent.y);
		}
		default:
			return false;
			break;
		}
	}
}

bool Gwen::Input::SDL2::ProcessKeyboardEvents(SDL_Event Event, Uint32 mWindowID)
{
	if (!m_Canvas)
		return false;

	//If an event was detected for this window
	if (Event.window.windowID == mWindowID)
	{
		switch (Event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		{
			SDL_KeyboardEvent keyEvent = Event.key;
			int iKey = -1;
			SDL_Scancode scancode = keyEvent.keysym.scancode;

			switch (scancode)
			{
			case SDL_SCANCODE_RETURN:
				iKey = Gwen::Key::Return;
				break;

			case SDL_SCANCODE_BACKSPACE:
				iKey = Gwen::Key::Backspace;
				break;

			case SDL_SCANCODE_DELETE:
				iKey = Gwen::Key::Delete;
				break;

			case SDL_SCANCODE_LEFT:
				iKey = Gwen::Key::Left;
				break;

			case SDL_SCANCODE_RIGHT:
				iKey = Gwen::Key::Right;
				break;

			case SDL_SCANCODE_LSHIFT:
				iKey = Gwen::Key::Shift;
				break;

			case SDL_SCANCODE_RSHIFT:
				iKey = Gwen::Key::Shift;
				break;

			case SDL_SCANCODE_TAB:
				iKey = Gwen::Key::Tab;
				break;

			case SDL_SCANCODE_SPACE:
				iKey = Gwen::Key::Space;
				break;

			case SDL_SCANCODE_HOME:
				iKey = Gwen::Key::Home;
				break;

			case SDL_SCANCODE_END:
				iKey = Gwen::Key::End;
				break;

			case SDL_SCANCODE_LCTRL:
				iKey = Gwen::Key::Control;
				break;

			case SDL_SCANCODE_RCTRL:
				iKey = Gwen::Key::Control;
				break;

			case SDL_SCANCODE_UP:
				iKey = Gwen::Key::Up;
				break;

			case SDL_SCANCODE_DOWN:
				iKey = Gwen::Key::Down;
				break;

			case SDL_SCANCODE_ESCAPE:
				iKey = Gwen::Key::Escape;
				break;

			case SDL_SCANCODE_LALT:
				iKey = Gwen::Key::Alt;
				break;

			case SDL_SCANCODE_RALT:
				iKey = Gwen::Key::Alt;
				break;

			default:
				return false;
			}

			return m_Canvas->InputKey(iKey, keyEvent.state);
		}

		case SDL_TEXTINPUT:
		{
			SDL_TextInputEvent txEvent = Event.text;
			if (txEvent.text[0] != '\0')
			{
				const char* toFormat = "UTF-32LE";
#if 0
				static SDL_iconv_t cd = SDL_iconv_t(-1);
				if (cd == SDL_iconv_t(-1))
				{
					cd = SDL_iconv_open(toFormat, "UTF-8");
					if (cd == SDL_iconv_t(-1))
					{
						printf("Couldn't initialize SDL_iconv for UTF-8 to UTF-32!"); // TODO: or error?
						return;
					}
				}

				Gwen::UnicodeChar n = (Gwen::UnicodeChar)SDL_iconv(cd, &txEvent.text, &inbytesleft, &outbuf, &outbytesleft);
				bool ret = m_Canvas->InputCharacter(n);

				// reset cd so it can be used again
				SDL_iconv_close(cd);
#else
				wchar_t* widechar = (wchar_t*)SDL_iconv_string(toFormat, "UTF-8", txEvent.text, SDL_strlen(txEvent.text) + 1);
				bool ret = m_Canvas->InputCharacter(*widechar);
				SDL_free(widechar);
#endif
			}
		}
		}
	}
}

bool Gwen::Input::SDL2::ProcessWindowEvents(SDL_Event Event, Uint32 mWindowID)
{
	if (!m_Canvas)
		return false;

	//If an event was detected for this window
	if (Event.type == SDL_WINDOWEVENT && Event.window.windowID == mWindowID)
	{
		switch (Event.window.event)
		{
			//Window appeared
		case SDL_WINDOWEVENT_SHOWN:
			m_Canvas->Show();
			break;

			//Window disappeared
		case SDL_WINDOWEVENT_HIDDEN:
			m_Canvas->SetHidden(true);
			return true;
			break;

			//Get new dimensions and repaint
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			return m_Canvas->SetSize(Gwen::Point(Event.window.data1, Event.window.data2));			 
			break;

			//Repaint on expose
		case SDL_WINDOWEVENT_EXPOSED:
			m_Canvas->Redraw();
			return true;
			break;

			//Mouse enter
		case SDL_WINDOWEVENT_ENTER:
			m_Canvas->SetMouseInputEnabled(true);
			return true;
			break;

			//Mouse exit
		case SDL_WINDOWEVENT_LEAVE:
			m_Canvas->SetMouseInputEnabled(false);
			return true;
			break;

			//Keyboard focus gained
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_Canvas->SetKeyboardInputEnabled(true);
			return true;
			break;

			//Keyboard focus lost
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_Canvas->SetKeyboardInputEnabled(false);
			return false;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			m_Canvas->SetHidden(true);
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			m_Canvas->SetHidden(false);
			return true;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			m_Canvas->SetHidden(false);
			return true;
			break;

			//Hide on close
		case SDL_WINDOWEVENT_CLOSE:
			return m_Canvas->InputQuit();
			break;

		default:
			return false;
		}
	}
}

bool Gwen::Input::SDL2::ProcessEvents(SDL_Event Event, Uint32 mWindowID)
{
	if (!m_Canvas)
		return false;

	//If an event was detected for this window
	if (Event.window.windowID == mWindowID)
	{
		switch (Event.type)
		{
		case SDL_QUIT:
		case SDL_APP_TERMINATING:
			m_Canvas->InputQuit();

		default:
			return false;
		}
	}
}

