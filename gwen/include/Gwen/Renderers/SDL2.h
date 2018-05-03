/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#ifndef GWEN_RENDERERS_SDL_H
#define GWEN_RENDERERS_SDL_H

#include "Gwen/Gwen.h"
#include "Gwen/Renderers/BaseRender.h"
#include <SDL.h>

namespace Gwen
{
	namespace Renderer
	{

		class SDL : public Gwen::Renderer::Base
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
				virtual void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text);
				
				void StartClip();
				void EndClip();

				virtual void LoadFont(Gwen::Font* pFont);
				virtual void FreeFont(Gwen::Font* pFont);
				virtual void LoadTexture( Gwen::Texture* pTexture );
				virtual void FreeTexture( Gwen::Texture* pTexture );

				virtual Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString & text);
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
