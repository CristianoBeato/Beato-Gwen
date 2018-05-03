#pragma once
#ifndef GWEN_SKINS_TEXTURING_H
#define GWEN_SKINS_TEXTURING_H

#include "Gwen/Gwen.h"

namespace Gwen
{
	//foward definition
	class Texture;
	
	namespace Renderer
	{
		class Base;
	};

	namespace Skin
	{
		namespace Texturing
		{
			struct Single
			{
				Single(void);

				void Init(Texture* pTexture, float x, float y, float w, float h);
				void Draw(Renderer::Base* render, Rect r, const Color & col = Colors::White);
				void DrawCenter(Renderer::Base* render, Rect r, const Color & col = Colors::White);

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
				void Draw(Renderer::Base* render, Gwen::Rect r, const Color & col = Colors::White, bool b1 = true, bool b2 = true, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true, bool b8 = true, bool b9 = true);
				void DrawRect(Renderer::Base* render, int i, int x, int y, int w, int h);

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
