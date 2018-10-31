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

#ifndef GWEN_BASERENDER_H
#define GWEN_BASERENDER_H

#include "Gwen/Structures.h"

namespace Gwen
{
	struct Font;
	class Texture;
	class WindowProvider;

	namespace Renderer
	{
		class Base;

		class ICacheToTexture
		{
			public:

				virtual ~ICacheToTexture() {}
				virtual void Initialize() = 0;
				virtual void ShutDown() = 0;
				virtual void SetupCacheTexture( Gwen::Controls::Base* control ) = 0;
				virtual void FinishCacheTexture( Gwen::Controls::Base* control ) = 0;
				virtual void DrawCachedControlTexture( Gwen::Controls::Base* control ) = 0;
				virtual void CreateControlCacheTexture( Gwen::Controls::Base* control ) = 0;
				virtual void UpdateControlCacheTexture( Gwen::Controls::Base* control ) = 0;
				virtual void SetRenderer( Gwen::Renderer::Base* renderer ) = 0;

		};

		class GWEN_EXPORT Base
		{
			public:

				Base();
				virtual ~Base();

				virtual void Init(void)  = 0;
				virtual void ShutDonw(void) = 0;
				virtual void Begin(void) = 0;
				virtual void End(void) = 0;

				virtual void SetDrawColor(Color color) { m_Color = color; };
				virtual void DrawFilledRect( Gwen::Rect rect ) =  0 ;

				virtual void StartClip(void) = 0;
				virtual void EndClip(void) = 0 ;

				virtual void LoadTexture( Gwen::Texture* pTexture ) = 0;
//Beato Begin: load texture texternaly
				virtual void LoadTexture(Gwen::Texture* pTexture, 
					unsigned int w, 
					unsigned int h,
					unsigned char bits,
					const char* col) = 0;
//Beato End;
				virtual void FreeTexture( Gwen::Texture* pTexture ) = 0;
				virtual void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f ) = 0;
				virtual void DrawMissingImage( Gwen::Rect pTargetRect );
				virtual Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default = Gwen::Color( 255, 255, 255, 255 ) ) { return col_default; }

				virtual ICacheToTexture* GetCTT() { return NULL; }

				virtual void LoadFont( Gwen::Font* pFont ) {};
				virtual void FreeFont( Gwen::Font* pFont ) {};
				virtual void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text );
				virtual Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString & text );

				// No need to implement these functions in your derived class, but if
				// you can do them faster than the default implementation it's a good idea to.

				virtual void DrawLinedRect( Gwen::Rect rect );
				virtual void DrawPixel( int x, int y );
				virtual void DrawShavedCornerRect( Gwen::Rect rect, bool bSlight = false );
				virtual Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::String & text );
				virtual void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::String & text );

		protected:
			Gwen::Color			m_Color;

			public:

				// Translate a panel's local drawing coordinate
				//  into view space, taking Offset's into account.
				void Translate( int & x, int & y );
				void Translate( Gwen::Rect & rect );

				// Set the rendering offset. You shouldn't have to
				// touch these, ever.
				void SetRenderOffset( const Gwen::Point & offset ) { m_RenderOffset = offset; }
				void AddRenderOffset( const Gwen::Rect & offset ) { m_RenderOffset.x += offset.x; m_RenderOffset.y += offset.y; }
				const Gwen::Point & GetRenderOffset() const { return m_RenderOffset; }

			private:

				Gwen::Point m_RenderOffset;

			public:

				void SetClipRegion( Gwen::Rect rect );
				void AddClipRegion( Gwen::Rect rect );
				bool ClipRegionVisible();
				const Gwen::Rect & ClipRegion() const;

			private:

				Gwen::Rect m_rectClipRegion;
				ICacheToTexture* m_RTT;

			public:

				void SetScale( float fScale ) { m_fScale = fScale; }
				float Scale() const { return m_fScale; }

				float m_fScale;


			public:

				//
				// Self Initialization, shutdown
				//

				virtual bool InitializeContext(Gwen::WindowProvider* pWindow) = 0;
				virtual bool ShutdownContext(Gwen::WindowProvider* pWindow) = 0;
				virtual bool ResizedContext(Gwen::WindowProvider* pWindow, int w, int h) = 0;

				virtual bool BeginContext(Gwen::WindowProvider* pWindow) = 0;
				virtual bool EndContext(Gwen::WindowProvider* pWindow) = 0;
				virtual bool PresentContext(Gwen::WindowProvider* pWindow) = 0;
		};
	}
}
#endif
