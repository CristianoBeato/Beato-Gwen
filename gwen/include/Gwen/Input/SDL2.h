/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/
#ifndef GWEN_INPUT_SDL2_H
#define GWEN_INPUT_SDL2_H

#include "Gwen/Gwen.h"
#include "Gwen/InputHandler.h"
#include "Gwen/Controls/Canvas.h"
#include <SDL_events.h>

namespace Gwen
{
	namespace Input
	{
		class SDL2
		{
			public:
				SDL2(void);
				void Initialize(Gwen::Controls::Canvas* c);
				bool ProcessWindowEvents(SDL_Event Event, Uint32 windowID);
				bool ProcessMouseEvents(SDL_Event Event, Uint32 mWindowID);
				bool ProcessKeyboardEvents(SDL_Event Event, Uint32 mWindowID);
				bool ProcessEvents(SDL_Event Event, Uint32 mWindowID);
		protected:
				Gwen::Controls::Canvas* m_Canvas;
		};
	}
}
#endif //!GWEN_INPUT_SDL2_H
