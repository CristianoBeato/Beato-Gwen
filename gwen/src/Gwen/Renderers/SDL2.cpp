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

#include "Gwen/Renderers/SDL2.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/WindowProvider.h"

#include <math.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#define _DEBUG_TEXTURE 0
#define _DEBUG_FONT 0

namespace Gwen
{
	namespace Renderer
	{
		SDL::SDL(void)
		{
			m_RenderWindow = NULL;
			m_RenderContext = NULL;
		}

		SDL::~SDL(void)
		{
		}

		void SDL::Init(void)
		{
		}
		
		void SDL::ShutDonw(void)
		{
		}

		void SDL::Begin(void)
		{
			//Check for render context
			if (!m_RenderContext)
				return;

			//Clear screen
			SDL_SetRenderDrawColor(m_RenderContext, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(m_RenderContext);
		}

		void SDL::End(void)
		{
		}

		void SDL::DrawFilledRect(Gwen::Rect rect)
		{
			Translate(rect);
			//Set rendering space and render to screen
			SDL_Rect fillRect = { rect.x, rect.y, rect.w, rect.h };
			//draw the rect
			SDL_RenderFillRect(m_RenderContext, &fillRect);
		}

		void SDL::DrawLinedRect(Gwen::Rect rect)
		{
			Translate(rect);
			//draw the line 
			SDL_RenderDrawLine(m_RenderContext, rect.x, rect.y, rect.w, rect.h);
		}

		void SDL::DrawPixel(int x, int y)
		{
			Translate(x, y);
			//draw the pixel point position
			SDL_RenderDrawPoint(m_RenderContext,x, y);
		}

		void SDL::DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2)
		{
			//SDL_Texture * texture = static_cast<SDL_Texture *>(pTexture->data);
			SDL_Texture * texture = pTexture->castData<SDL_Texture>();
			if (!texture)
				return;

			Translate(rect);

			//Set rendering space and render to screen
			SDL_Rect fillRect = { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };

			//convert the UV to SDL clip space
			float fW = (float)pTexture->getWidth();
			float fH = (float)pTexture->getHeight();
			u1 *= fW;
			v1 *= fH;
			u2 *= fW;
			u2 -= u1;
			v2 *= fH;
			v2 -= v1;
			
			//Set rendering space and render to screen
			SDL_Rect clip = { (int)u1, (int)v1, (int)u2, (int)v2 };
			
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

			//Modulate texture rgb
			SDL_SetTextureColorMod(texture, m_Color.r, m_Color.g, m_Color.b);

			//Render to screen
			SDL_RenderCopy(m_RenderContext, texture, &clip, &fillRect);

#if _DEBUG_TEXTURE
			//Rendet a Red outiline of the texture rect bouds
			SDL_SetRenderDrawColor(m_RenderContext, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(m_RenderContext, &fillRect);
#endif
		}

		void SDL::RenderText(Gwen::Font * pFont, Gwen::Point pos, const Gwen::UnicodeString & text)
		{
			Gwen::Rect rect;
			SDL_Surface* textSurface = NULL;
			// If the font doesn't exist, or the font size should be changed
			if (!pFont->data || fabs(pFont->realsize - pFont->size * Scale()) > 2)
			{
				FreeFont(pFont);
				LoadFont(pFont);
			}

			TTF_Font *gFont = static_cast<TTF_Font*>(pFont->data);
			if (!gFont)
				return;

			SDL_Color textFgColor = { m_Color.r, m_Color.g, m_Color.b, m_Color.a};
			
			//Render text surface
#if 0
			textSurface = TTF_RenderText_Blended(gFont, Gwen::Utility::UnicodeToString(text).c_str(), textFgColor);
#else
			textSurface = TTF_RenderUNICODE_Blended(gFont, (Uint16*)text.c_str(), textFgColor);
#endif
			if (textSurface == NULL)
			{
				printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
				return;
			}

			rect = Rect(pos.x, pos.y, textSurface->w, textSurface->h);
			Translate(rect);
			
			//Create texture from surface pixels
			SDL_Texture *mTexture = SDL_CreateTextureFromSurface(m_RenderContext, textSurface);
			if (mTexture == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
				//Get rid of old surface
				SDL_FreeSurface(textSurface);
				return;
			}

			//Set rendering space and render to screen
			SDL_Rect fillRect = { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };

			//Set rendering space and render to screen
			SDL_Rect clip = { 0, 0, (int)rect.w, (int)rect.h };

			SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

			//Render to screen
			SDL_RenderCopy(m_RenderContext, mTexture, &clip, &fillRect);

#if _DEBUG_FONT
			//Rendet a Red outiline of the texture rect bouds
			SDL_SetRenderDrawColor(m_RenderContext, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(m_RenderContext, &fillRect);
#endif

			//Get rid of old surface
			SDL_DestroyTexture(mTexture);
			SDL_FreeSurface(textSurface);
		}

		void SDL::SetDrawColor(Gwen::Color color)
		{
			Base::SetDrawColor(color);
			//set the current rect color
			SDL_SetRenderDrawColor(m_RenderContext, color.r, color.g, color.b, color.a);
		}

		void SDL::StartClip(void)
		{
			const Gwen::Rect rect = ClipRegion();
		
			//set the clipping area 
			SDL_Rect clip { rect.x, rect.y, rect.w, rect.h };
			SDL_RenderSetClipRect(m_RenderContext, &clip);
		};

		void SDL::EndClip(void)
		{
			SDL_Rect clip;
			//get the clip and set to clip only the selected area 
			SDL_RenderGetViewport(m_RenderContext, &clip);

			//clip all the view port 
			SDL_RenderSetClipRect(m_RenderContext, &clip);
		}

		void SDL::LoadFont(Gwen::Font * pFont)
		{
			TTF_Font *NewFont = TTF_OpenFont(Gwen::Utility::UnicodeToString(pFont->facename).c_str(), pFont->size);

			if (NewFont == NULL)
			{
				printf("Unable to load font %s! TTF_OpenFont Error: %s\n", pFont->facename.c_str(), SDL_GetError());
				return;
			}
			pFont->data = static_cast<void*>(NewFont);
		}

		void SDL::FreeFont(Gwen::Font * pFont)
		{
			TTF_Font *oldFont = static_cast<TTF_Font*>(pFont->data);
			if (oldFont)
			{
				TTF_CloseFont(oldFont);
				pFont->data = NULL;
			}
		}
		
		void SDL::LoadTexture(Gwen::Texture* pTexture,
			unsigned int w,
			unsigned int h,
			unsigned char bits,
			const char* col)
		{
			Uint32 rmask, gmask, bmask, amask;
			int depth, pitch;
			//The final texture
			SDL_Texture* newTexture = NULL;

			// Set up the pixel format color masks for RGB(A) byte arrays.
			// Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
#else // little endian, like x86
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
#endif
				
			//8 bit color precision
			depth = 8 * bits;
			pitch = bits * w; // 3 bytes per pixel * pixels per row
	
			//Load image at specified path		
			SDL_Surface* loadedSurface = SDL_CreateRGBSurfaceFrom((void*)col, w, h, depth, pitch, rmask, gmask, bmask, amask);

			if (loadedSurface == NULL)
			{
				printf("Unable to load image %s! SDL_image Error: %s\n", pTexture->getName().c_str(), SDL_GetError());
				return;
			}

			//Color key image
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(m_RenderContext, loadedSurface);
			if (newTexture == NULL)
			{
				//Get rid of old loaded surface
				SDL_FreeSurface(loadedSurface);

				printf("Unable to create texture from %s! SDL Error: %s\n", pTexture->getName().c_str(), SDL_GetError());
				return;
			}

			//Get image dimensions
			pTexture->getWidth() = w;
			pTexture->getHeight() = h;

			//Return success
			pTexture->getData() = static_cast<void*>(newTexture);
		}
		
		void SDL::LoadTexture(Gwen::Texture* pTexture)
		{
			//The final texture
			SDL_Texture* newTexture = NULL;
				
			//Load image at specified path		
			SDL_Surface* loadedSurface = SDL_LoadBMP(pTexture->getName().c_str());
			if (loadedSurface == NULL)
			{
				printf("Unable to load image %s! SDL_image Error: %s\n", pTexture->getName().c_str(), SDL_GetError());
				return;
			}

			//Color key image
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(m_RenderContext, loadedSurface);
			if (newTexture == NULL)
			{
				//Get rid of old loaded surface
				SDL_FreeSurface(loadedSurface);

				printf("Unable to create texture from %s! SDL Error: %s\n", pTexture->getName().c_str(), SDL_GetError());
				return;
			}
				
			//Get image dimensions
			pTexture->getWidth() = loadedSurface->w;
			pTexture->getHeight() = loadedSurface->h;
		
			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);

			SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

			//Return success
			pTexture->getData() = static_cast<void*>(newTexture);
		}

		void SDL::FreeTexture(Gwen::Texture* pTexture)
		{
			if (!pTexture)
				return;

			SDL_DestroyTexture(pTexture->castData<SDL_Texture>());
		}

		Gwen::Point SDL::MeasureText(Gwen::Font * pFont, const Gwen::UnicodeString & text)
		{
			int w, h;
			// If the font doesn't exist, or the font size should be changed
			if (!pFont->data || fabs(pFont->realsize - pFont->size * Scale()) > 2)
			{
				FreeFont(pFont);
				LoadFont(pFont);
			}

			TTF_Font *gFont = static_cast<TTF_Font*>(pFont->data);
			if (!gFont)
				return Gwen::Point();

			if (TTF_SizeText(gFont, Gwen::Utility::UnicodeToString(text).c_str(), &w, &h) > 0)
			{
				printf("Error on TTF_SizeText, SDL Error: %s\n", SDL_GetError());
				return Gwen::Point();
			}
			return Gwen::Point(w, h);
		}

		Gwen::Color SDL::PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default)
		{
			//if (pTexture != nullptr)
			//{
				//SLOW AND WRONG 
				//SDL_Rect clip = {x, y, 1, 1};
				//unsigned char pixels[4];
				//SDL_RenderReadPixels(m_RenderContext, &clip, 0, pixels, 0);
				//return Gwen::Color(pixels[0], pixels[1], pixels[2], pixels[3]);
			//}

			return col_default;
		}

		bool SDL::InitializeContext(Gwen::WindowProvider* pWindow)
		{
			m_RenderWindow = static_cast<SDL_Window*>(pWindow->GetWindow());
			if (!m_RenderWindow)
				return false;

			m_RenderContext = SDL_CreateRenderer(m_RenderWindow, -1, SDL_RENDERER_ACCELERATED |
																	SDL_RENDERER_PRESENTVSYNC |
																	SDL_RENDERER_TARGETTEXTURE);

			if (m_RenderContext == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
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

		bool SDL::ShutdownContext(Gwen::WindowProvider* pWindow)
		{
			bool status = false; 
			TTF_Quit();
			if (m_RenderContext)
			{
				SDL_DestroyRenderer(m_RenderContext);
				m_RenderContext = NULL;
			}
			else
				status = false;

			return status;
		}

		bool SDL::PresentContext(Gwen::WindowProvider* pWindow)
		{
			//Update screen
			SDL_RenderPresent(m_RenderContext);
			return true;
		}

		bool SDL::ResizedContext(Gwen::WindowProvider* pWindow, int w, int h)
		{
			return true;
		}

		bool SDL::BeginContext(Gwen::WindowProvider* pWindow)
		{
			return true;
		}

		bool SDL::EndContext(Gwen::WindowProvider* pWindow)
		{
			return true;
		}
	}
}