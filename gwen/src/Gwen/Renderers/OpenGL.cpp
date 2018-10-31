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

#include "Gwen/Renderers/OpenGL.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/WindowProvider.h"

#include <math.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#define _DEBUG_TEXTURE 0
#define _DEBUG_FONT 0

namespace Gwen
{
	namespace Renderer
	{
		OpenGL::OpenGL()
		{
			m_RenderWindow = NULL;
			m_RenderContext = NULL;
		}

		OpenGL::~OpenGL(void)
		{
		}

		void OpenGL::Init(void)
		{
		}
		
		void OpenGL::ShutDonw(void)
		{
		}

		void OpenGL::Begin(void)
		{
			//clear buffers 
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void OpenGL::End(void)
		{
			glFlush();
		}

		void OpenGL::SetDrawColor(Gwen::Color color)
		{
		}

		void OpenGL::DrawFilledRect(Gwen::Rect rect)
		{
		}

		void OpenGL::DrawLinedRect(Gwen::Rect rect)
		{
		}

		void OpenGL::DrawPixel(int x, int y)
		{
		}

		void OpenGL::DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2)
		{
		}

		void OpenGL::RenderText(Gwen::Font * pFont, Gwen::Point pos, const Gwen::UnicodeString & text)
		{
		}

		void OpenGL::StartClip()
		{
		};

		void OpenGL::EndClip()
		{
		}

		void OpenGL::LoadFont(Gwen::Font * pFont)
		{
			TTF_Font *NewFont = TTF_OpenFont(Gwen::Utility::UnicodeToString(pFont->facename).c_str(), pFont->size);

			if (NewFont == NULL)
			{
				printf("Unable to load font %s! TTF_OpenFont Error: %s\n", pFont->facename.c_str(), SDL_GetError());
				return;
			}
			pFont->data = static_cast<void*>(NewFont);
		}

		void OpenGL::FreeFont(Gwen::Font * pFont)
		{
			TTF_Font *oldFont = static_cast<TTF_Font*>(pFont->data);
			if (oldFont)
			{
				TTF_CloseFont(oldFont);
				pFont->data = NULL;
			}
		}
		
		void OpenGL::LoadTexture(Gwen::Texture* pTexture)
		{
		}

		void OpenGL::FreeTexture(Gwen::Texture* pTexture)
		{
		}

	//	Gwen::Point OpenGL::MeasureText(Gwen::Font * pFont, const Gwen::UnicodeString & text)
	//	{
	//	}

	//	Gwen::Color OpenGL::PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color &/ col_default)
	//	{
	//	}

		bool OpenGL::InitializeContext(Gwen::WindowProvider* pWindow)
		{
			m_RenderWindow = static_cast<SDL_Window*>(pWindow->GetWindow());
			if (!m_RenderWindow)
				return false;

			// Create an OpenGL context associated with the window.
			m_RenderContext = SDL_GL_CreateContext(m_RenderWindow);

			if (m_RenderContext == NULL)
			{
				printf("Renderer could not be created! OpenGL Error: %s\n", SDL_GetError());
				return false;
			}
			
			if (TTF_WasInit() < 1)
			{
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					return false;
				}
			}

			return true;
		}

		bool OpenGL::ShutdownContext(Gwen::WindowProvider* pWindow)
		{
			bool status = false; 
			TTF_Quit();
			if (m_RenderContext)
			{
				// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
				SDL_GL_DeleteContext(m_RenderContext);
			}
			else
				status = false;

			return status;
		}

		bool OpenGL::PresentContext(Gwen::WindowProvider* pWindow)
		{
			m_RenderWindow = static_cast<SDL_Window*>(pWindow->GetWindow());
			if (!m_RenderWindow)
				return false;

			//present the context render 
			SDL_GL_SwapWindow(m_RenderWindow);
			return true;
		}

		bool OpenGL::ResizedContext(Gwen::WindowProvider* pWindow, int w, int h)
		{
			return true;
		}

		bool OpenGL::BeginContext(Gwen::WindowProvider* pWindow)
		{
			return true;
		}

		bool OpenGL::EndContext(Gwen::WindowProvider* pWindow)
		{
			return true;
		}
	}
}