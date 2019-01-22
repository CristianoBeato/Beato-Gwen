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

#ifndef GWEN_RENDERERS_SDL_H
#define GWEN_RENDERERS_SDL_H

#include "Gwen/Gwen.h"
#include "Gwen/Renderers/BaseRender.h"
#include "Gwen/Fonts/Font.h"
#include <SDL_render.h>

namespace Gwen
{
	namespace Renderer
	{
		class SDL : public BaseRender
		{
			public:
				SDL();
				~SDL();

				virtual void Init(void);
				virtual void ShutDonw(void);
				virtual void Begin(void);
				virtual void End(void);

				virtual void SetDrawColor( Gwen::Color color );
				virtual void DrawFilledRect( Gwen::Rect rect );
				virtual void DrawLinedRect(Gwen::Rect rect);
				virtual void DrawPixel(int x, int y);
				virtual void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );
				virtual void RenderText(Gwen::Font::FontBase* pFont, Gwen::Point pos, const Gwen::UnicodeString & text);
				
				void StartClip();
				void EndClip();

				virtual void LoadFont(Gwen::Font::FontBase* pFont);
				virtual void FreeFont(Gwen::Font::FontBase* pFont);
				virtual void LoadTexture( Gwen::Texture* pTexture );
				virtual void FreeTexture( Gwen::Texture* pTexture );
				virtual void LoadTexture(Gwen::Texture* pTexture, Uint32 w, Uint32 h, Uint8 bits,
										const char* col);

				virtual Gwen::Point MeasureText(Gwen::Font::FontBase* pFont, const Gwen::UnicodeString & text);
				Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default );

			protected:
				
				//the main APP window
				SDL_Window		*m_RenderWindow;
				//The SDL Render context
				SDL_Renderer	*m_RenderContext;
			public:

				// Self Initialization
				virtual bool InitializeContext( Gwen::WindowProvider* pWindow = NULL);
				virtual bool ShutdownContext( Gwen::WindowProvider* pWindow = NULL);
				virtual bool PresentContext( Gwen::WindowProvider* pWindow = NULL);
				virtual bool ResizedContext( Gwen::WindowProvider* pWindow = NULL, int w = 0, int h = 0);
				virtual bool BeginContext( Gwen::WindowProvider* pWindow = NULL);
				virtual bool EndContext( Gwen::WindowProvider* pWindow = NULL);
		};

	}
}
#endif
