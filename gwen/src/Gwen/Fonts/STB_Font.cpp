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

#include "Gwen/Fonts/STB_Font.h"

Gwen::Font::STBFont::STBFont(void)
{
}

void Gwen::Font::STBFont::Load(Gwen::Renderer::BaseRender * render)
{
	// Load font.
	FILE* fp = fopen(Gwen::Utility::UnicodeToString(m_facename).c_str(), "rb");
	if (!fp)
		return;
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fclose(fp);
		return;
	}

	long size = ftell(fp);
	if (size < 0)
	{
		fclose(fp);
		return;
	}

	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fclose(fp);
		return;
	}
	
	byte* ttfBuffer = (byte*)SDL_malloc(size);
	if (!ttfBuffer)
	{
		fclose(fp);
		return;
	}

	size_t readLen = fread(ttfBuffer, 1, size, fp);
	fclose(fp);
	if (readLen != static_cast<size_t>(size))
	{
		SDL_free(ttfBuffer);
		return;
	}

	fp = 0;

	m_AtlasWidth = 512;
	m_AtlasHeight = 512;
	byte* colorBuff = (byte*)SDL_malloc(m_AtlasWidth * m_AtlasHeight);
	if (!m_AtlasBuff)
	{
		SDL_free(ttfBuffer);
		return;
	}

	stbtt_BakeFontBitmap(ttfBuffer, 0, m_size, m_AtlasBuff, m_AtlasWidth, m_AtlasHeight, 32, 96, m_cdata);

	m_AtlasDepth = 32;
	m_AtlasBuff = (byte*)SDL_malloc(m_AtlasWidth * m_AtlasHeight * 4);
	for (Uint32 h = 0; h < m_AtlasHeight; h++)
	{
		for (Uint32 w = 0; w < m_AtlasWidth; w++)
		{
			byte pixel = *colorBuff++;
			*m_AtlasBuff++ = pixel;
			*m_AtlasBuff++ = pixel;
			*m_AtlasBuff++ = pixel;
			*m_AtlasBuff++ = pixel;
		}
	}
	SDL_free(colorBuff);

	//send tha font map to the render
	render->LoadFont(this);

	SDL_free(ttfBuffer);
	SDL_free(m_AtlasBuff);

}

static const int k_tabStops[4] = { 150, 210, 270, 330 };
float Gwen::Font::STBFont::GetTextLength(const Gwen::UnicodeString & string)
{
	int xpos = 0;
	int len = 0;
	const char* text = Gwen::Utility::UnicodeToString(string).c_str();
	while (*text)
	{
		int c = (unsigned char)*text;
		if (c == '\t')
		{
			for (int i = 0; i < 4; ++i)
			{
				if (xpos < k_tabStops[i])
				{
					xpos = k_tabStops[i];
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = m_cdata + c - 32;
			int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
			len = round_x + b->x1 - b->x0 + 0.5f;
			xpos += b->xadvance;
		}
		++text;
	}
	return len;
}

void Gwen::Font::STBFont::GetCharRenderRect(int char_index, float &xpos, float &ypos, CharRect & rect)
{
	stbtt_bakedchar *b = m_cdata + char_index;
	int round_x = STBTT_ifloor(xpos + b->xoff);
	int round_y = STBTT_ifloor(ypos - b->yoff);

	rect.x = (float)round_x;
	rect.y = (float)round_y;
	rect.w = (float)round_x + b->x1 - b->x0;
	rect.h = (float)round_y - b->y1 + b->y0;

	rect.s0 = b->x0 / (float)m_AtlasWidth;
	rect.t0 = b->y0 / (float)m_AtlasWidth;
	rect.s1 = b->x1 / (float)m_AtlasHeight;
	rect.t1 = b->y1 / (float)m_AtlasHeight;

	xpos += b->xadvance;
}