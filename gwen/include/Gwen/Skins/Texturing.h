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

#ifndef GWEN_SKINS_TEXTURING_H
#define GWEN_SKINS_TEXTURING_H

#include "Gwen/Gwen.h"
#include "Gwen/texture/Texture.h"

namespace Gwen
{
	namespace Skin
	{
		namespace Texturing
		{
			struct Single
			{
				Single(void);
				void Init(Texture* pTexture, float x, float y, float w, float h);
				void Draw(Gwen::Renderer::BaseRender* render, Gwen::Rect r, const Gwen::Color & col = Gwen::Colors::White);
				void DrawCenter(Gwen::Renderer::BaseRender* render, Gwen::Rect r, const Gwen::Color & col = Gwen::Colors::White);

				Texture*	texture;
				float		uv[4];
				int			iWidth;
				int			iHeight;
			};

			struct Bordered
			{
				Bordered(void);
				void Init(Texture* pTexture, float x, float y, float w, float h, Margin in_margin, float DrawMarginScale = 1.0f);
				void SetRect(int iNum, float x, float y, float w, float h);
				void Draw(Gwen::Renderer::BaseRender* render, Gwen::Rect r, const Gwen::Color & col = Gwen::Colors::White, bool b1 = true, bool b2 = true, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true, bool b8 = true, bool b9 = true);
				void DrawRect(Gwen::Renderer::BaseRender* render, int i, int x, int y, int w, int h);

				Texture*	texture;

				struct SubRect
				{
					float uv[4];
				};

				SubRect rects[9];
				Margin margin;

				float width;
				float height;
			};
		}
	}
}
#endif
