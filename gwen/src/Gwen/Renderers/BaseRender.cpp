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

#include <math.h>
#include "Gwen/Renderers/BaseRender.h"
#include "Gwen/Fonts/Font.h"

Gwen::Renderer::BaseRender::BaseRender()
{
	m_RenderOffset = Gwen::Point(0, 0);
	m_fScale = 1.0f;
}

Gwen::Renderer::BaseRender::~BaseRender(void)
{
	if (GetCTT())
	{
		GetCTT()->ShutDown();
	}
}

void Gwen::Renderer::BaseRender::RenderText(Gwen::Font::FontBase* pFont, Gwen::Point pos, const Gwen::String & text)
{
	Gwen::UnicodeString str = Gwen::Utility::StringToUnicode(text);
	RenderText(pFont, pos, str);
}

Gwen::Point Gwen::Renderer::BaseRender::MeasureText(Gwen::Font::FontBase* pFont, const Gwen::String & text)
{
	Gwen::UnicodeString str = Gwen::Utility::StringToUnicode(text);
	return MeasureText(pFont, str);
}

void Gwen::Renderer::BaseRender::DrawLinedRect(Gwen::Rect rect)
{
	DrawFilledRect(Gwen::Rect(rect.x, rect.y, rect.w, 1));
	DrawFilledRect(Gwen::Rect(rect.x, rect.y + rect.h - 1, rect.w, 1));
	DrawFilledRect(Gwen::Rect(rect.x, rect.y, 1, rect.h));
	DrawFilledRect(Gwen::Rect(rect.x + rect.w - 1, rect.y, 1, rect.h));
}

void Gwen::Renderer::BaseRender::DrawPixel(int x, int y)
{
	DrawFilledRect(Gwen::Rect(x, y, 1, 1));
}

void Gwen::Renderer::BaseRender::DrawShavedCornerRect(Gwen::Rect rect, bool bSlight)
{
	// Draw INSIDE the w/h.
	rect.w -= 1;
	rect.h -= 1;

	if (bSlight)
	{
		DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y, rect.w - 1, 1));
		DrawFilledRect(Gwen::Rect(rect.x + 1, rect.y + rect.h, rect.w - 1, 1));
		DrawFilledRect(Gwen::Rect(rect.x, rect.y + 1, 1, rect.h - 1));
		DrawFilledRect(Gwen::Rect(rect.x + rect.w, rect.y + 1, 1, rect.h - 1));
		return;
	}

	DrawPixel(rect.x + 1, rect.y + 1);
	DrawPixel(rect.x + rect.w - 1, rect.y + 1);
	DrawPixel(rect.x + 1, rect.y + rect.h - 1);
	DrawPixel(rect.x + rect.w - 1, rect.y + rect.h - 1);
	DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y, rect.w - 3, 1));
	DrawFilledRect(Gwen::Rect(rect.x + 2, rect.y + rect.h, rect.w - 3, 1));
	DrawFilledRect(Gwen::Rect(rect.x, rect.y + 2, 1, rect.h - 3));
	DrawFilledRect(Gwen::Rect(rect.x + rect.w, rect.y + 2, 1, rect.h - 3));
}

void Gwen::Renderer::BaseRender::Translate(int & x, int & y)
{
	x += m_RenderOffset.x;
	y += m_RenderOffset.y;
	x = ceilf(((float)x) * m_fScale);
	y = ceilf(((float)y) * m_fScale);
}

void Gwen::Renderer::BaseRender::Translate(Gwen::Rect & rect)
{
	Translate(rect.x, rect.y);
	rect.w = ceilf(((float)rect.w) * m_fScale);
	rect.h = ceilf(((float)rect.h) * m_fScale);
}

void Gwen::Renderer::BaseRender::SetClipRegion(Gwen::Rect rect)
{
	m_rectClipRegion = rect;
}

void Gwen::Renderer::BaseRender::AddClipRegion(Gwen::Rect rect)
{
	rect.x = m_RenderOffset.x;
	rect.y = m_RenderOffset.y;
	Gwen::Rect out = rect;

	if (rect.x < m_rectClipRegion.x)
	{
		out.w -= (m_rectClipRegion.x - out.x);
		out.x = m_rectClipRegion.x;
	}

	if (rect.y < m_rectClipRegion.y)
	{
		out.h -= (m_rectClipRegion.y - out.y);
		out.y = m_rectClipRegion.y;
	}

	if (rect.x + rect.w > m_rectClipRegion.x + m_rectClipRegion.w)
	{
		out.w = (m_rectClipRegion.x + m_rectClipRegion.w) - out.x;
	}

	if (rect.y + rect.h > m_rectClipRegion.y + m_rectClipRegion.h)
	{
		out.h = (m_rectClipRegion.y + m_rectClipRegion.h) - out.y;
	}

	m_rectClipRegion = out;
}

const Gwen::Rect & Gwen::Renderer::BaseRender::ClipRegion() const
{
	return m_rectClipRegion;
}

bool Gwen::Renderer::BaseRender::ClipRegionVisible()
{
	if (m_rectClipRegion.w <= 0 || m_rectClipRegion.h <= 0)
	{
		return false;
	}

	return true;
}

void Gwen::Renderer::BaseRender::DrawMissingImage(Gwen::Rect pTargetRect)
{
	SetDrawColor(Colors::Red);
	DrawFilledRect(pTargetRect);
}

Gwen::Color Gwen::Renderer::BaseRender::PixelColour(Gwen::Texture * pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default) { return col_default; }


		/*
			If they haven't defined these font functions in their renderer code
			we just draw some rects where the letters would be to give them an idea.
		*/

void Gwen::Renderer::BaseRender::RenderText(Gwen::Font::FontBase* pFont, Gwen::Point pos, const Gwen::UnicodeString & text)
{
	float fSize = pFont->Size() * Scale();

	for (float i = 0; i < text.length(); i++)
	{
		wchar_t chr = text[i];

		if (chr == ' ')
			continue;

		Gwen::Rect r(pos.x + i * fSize * 0.4, pos.y, fSize * 0.4 - 1, fSize);

		/*
			This isn't important, it's just me messing around changing the
			shape of the rect based on the letter.. just for fun.
		*/
		if (chr == 'l' || chr == 'i' || chr == '!' || chr == 't')
		{
			r.w = 1;
		}
		else if (chr >= 'a' && chr <= 'z')
		{
			r.y += fSize * 0.5f;
			r.h -= fSize * 0.4f;
		}
		else if (chr == '.' || chr == ',')
		{
			r.x += 2;
			r.y += r.h - 2;
			r.w = 2;
			r.h = 2;
		}
		else if (chr == '\'' || chr == '`' || chr == '"')
		{
			r.x += 3;
			r.w = 2;
			r.h = 2;
		}

		if (chr == 'o' || chr == 'O' || chr == '0')
		{
			DrawLinedRect(r);
		}
		else
		{
			DrawFilledRect(r);
		}
	}
}

Gwen::Point Gwen::Renderer::BaseRender::MeasureText(Gwen::Font::FontBase* pFont, const Gwen::UnicodeString & text)
{
	Gwen::Point p;
	p.x = pFont->Size() * Scale() * (float)text.length() * 0.4;
	p.y = pFont->Size() * Scale();
	return p;
}

void Gwen::Renderer::BaseRender::SetDrawColor(Color color) { m_Color = color; }
