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

#ifndef GWEN_RENDERERS_OPENGL_H
#define GWEN_RENDERERS_OPENGL_H

#include <SDL_render.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include "Gwen/Gwen.h"
#include "Gwen/Renderers/BaseRender.h"


namespace Gwen
{
	namespace Renderer
	{
		class OpenGL : public BaseRender
		{
			struct vertex
			{
				float	positions[2];
				float	textcoord[2];
				Uint8	vertColor[4];
			};
				
			public:
				OpenGL();
				~OpenGL();

				virtual void Init(void);
				virtual void ShutDonw(void);

				virtual void Begin(void);
				virtual void End(void);


				void StartClip();
				void EndClip();

				virtual void DrawFilledRect( Gwen::Rect rect );
				virtual void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );
				virtual void LoadTexture( Gwen::Texture* pTexture );
				virtual void LoadTexture(Gwen::Texture* pTexture, Uint32 w, Uint32 h, Uint8 bits, const char* col);
				virtual void FreeTexture( Gwen::Texture* pTexture );
				Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default );

				virtual void LoadFont(Gwen::Font::FontBase* pFont);
				virtual void FreeFont(Gwen::Font::FontBase* pFont);
				virtual void RenderText(Gwen::Font::FontBase* pFont, Gwen::Point pos, const Gwen::UnicodeString & text);
				virtual Gwen::Point MeasureText(Gwen::Font::FontBase* pFont, const Gwen::UnicodeString & text);

			protected:
				static const Uint16 k_maxVertex = 1024;

				void			Flush(void); //present elements top
				void			AddVert(float x, float y, float u = 0.0f, float v = 0.0f);
				void			BindTexture(Gwen::Texture* pTexture);
				void			UnbindTexture(void);
				
				Uint16			m_Width, m_Height;
				Uint16			m_numVertexes;
				vertex			m_Vertices[k_maxVertex];
				
				Uint8			m_lastDraw;

				GLuint			m_VAO, m_VBO;
				GLuint			vsProgram, fsProgram;
				GLuint			m_uTexture, m_ublendMode;
				GLuint			pipeline;

			public:

				//
				// Self Initialization
				//

				virtual bool InitializeContext( Gwen::WindowProvider* pWindow );
				virtual bool ShutdownContext( Gwen::WindowProvider* pWindow );
				virtual bool PresentContext( Gwen::WindowProvider* pWindow );
				virtual bool ResizedContext( Gwen::WindowProvider* pWindow, int w, int h );
				virtual bool BeginContext( Gwen::WindowProvider* pWindow );
				virtual bool EndContext( Gwen::WindowProvider* pWindow );

				void*	m_pContext;
		};

	}
}
#endif