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

#include "Gwen/Skins/Texturing.h"

Gwen::Skin::Texturing::Single::Single(void)
{
	texture = NULL;
}

void Gwen::Skin::Texturing::Single::Init(Texture * pTexture, float x, float y, float w, float h)
{
	texture = pTexture;
	float texw = texture->getWidth();
	float texh = texture->getHeight();
	uv[0] = x / texw;
	uv[1] = y / texh;
	uv[2] = (x + w) / texw;
	uv[3] = (y + h) / texh;
	this->iWidth = w;
	this->iHeight = h;
}

void Gwen::Skin::Texturing::Single::Draw(Renderer::Base * render, Rect r, const Color & col)
{
	if (!texture) { return; }

	render->SetDrawColor(col);
	render->DrawTexturedRect(texture, r, uv[0], uv[1], uv[2], uv[3]);
}

void Gwen::Skin::Texturing::Single::DrawCenter(Renderer::Base * render, Rect r, const Color & col)
{
	if (!texture) { return; }

	r.x += (r.w - iWidth) * 0.5;
	r.y += (r.h - iHeight) * 0.5;
	r.w = iWidth;
	r.h = iHeight;
	Draw(render, r, col);
}

Gwen::Skin::Texturing::Bordered::Bordered()
{
	texture = NULL;
}

void Gwen::Skin::Texturing::Bordered::Init(Texture * pTexture, float x, float y, float w, float h, Margin in_margin, float DrawMarginScale)
{
	texture = pTexture;
	margin = in_margin;
	SetRect(0, x, y, margin.left, margin.top);
	SetRect(1, x + margin.left, y, w - margin.left - margin.right, margin.top);
	SetRect(2, (x + w) - margin.right, y, margin.right, margin.top);
	SetRect(3, x, y + margin.top, margin.left, h - margin.top - margin.bottom);
	SetRect(4, x + margin.left, y + margin.top, w - margin.left - margin.right, h - margin.top - margin.bottom);
	SetRect(5, (x + w) - margin.right, y + margin.top, margin.right, h - margin.top - margin.bottom);
	SetRect(6, x, (y + h) - margin.bottom, margin.left, margin.bottom);
	SetRect(7, x + margin.left, (y + h) - margin.bottom, w - margin.left - margin.right, margin.bottom);
	SetRect(8, (x + w) - margin.right, (y + h) - margin.bottom, margin.right, margin.bottom);
	margin.left *= DrawMarginScale;
	margin.right *= DrawMarginScale;
	margin.top *= DrawMarginScale;
	margin.bottom *= DrawMarginScale;
	width = w - x;
	height = h - y;
}

void Gwen::Skin::Texturing::Bordered::SetRect(int iNum, float x, float y, float w, float h)
{
	float texw = texture->getWidth();
	float texh = texture->getHeight();
	//x -= 1.0f;
	//y -= 1.0f;
	rects[iNum].uv[0] = x / texw;
	rects[iNum].uv[1] = y / texh;
	rects[iNum].uv[2] = (x + w) / texw;
	rects[iNum].uv[3] = (y + h) / texh;
	//	rects[iNum].uv[0] += 1.0f / texture->width;
	//	rects[iNum].uv[1] += 1.0f / texture->width;
}

void Gwen::Skin::Texturing::Bordered::Draw(Gwen::Renderer::Base * render, Gwen::Rect r, const Gwen::Color & col, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, bool b8, bool b9)
{
	if (!texture) { return; }

	render->SetDrawColor(col);

	if (r.w < width && r.h < height)
	{
		render->DrawTexturedRect(texture,
			r,
			rects[0].uv[0], rects[0].uv[1], rects[8].uv[2], rects[8].uv[3]);
		return;
	}

	if (b1) { DrawRect(render, 0, r.x, r.y, margin.left, margin.top); }

	if (b2) { DrawRect(render, 1, r.x + margin.left, r.y, r.w - margin.left - margin.right, margin.top); }

	if (b3) { DrawRect(render, 2, (r.x + r.w) - margin.right, r.y, margin.right, margin.top); }

	if (b4) { DrawRect(render, 3, r.x, r.y + margin.top, margin.left, r.h - margin.top - margin.bottom); }

	if (b5) { DrawRect(render, 4, r.x + margin.left, r.y + margin.top, r.w - margin.left - margin.right, r.h - margin.top - margin.bottom); }

	if (b6) { DrawRect(render, 5, (r.x + r.w) - margin.right, r.y + margin.top, margin.right, r.h - margin.top - margin.bottom); }

	if (b7) { DrawRect(render, 6, r.x, (r.y + r.h) - margin.bottom, margin.left, margin.bottom); }

	if (b8) { DrawRect(render, 7, r.x + margin.left, (r.y + r.h) - margin.bottom, r.w - margin.left - margin.right, margin.bottom); }

	if (b9) { DrawRect(render, 8, (r.x + r.w) - margin.right, (r.y + r.h) - margin.bottom, margin.right, margin.bottom); }
}

void Gwen::Skin::Texturing::Bordered::DrawRect(Gwen::Renderer::Base * render, int i, int x, int y, int w, int h)
{
	render->DrawTexturedRect(texture,
		Gwen::Rect(x, y, w, h),
		rects[i].uv[0], rects[i].uv[1], rects[i].uv[2], rects[i].uv[3]);
}
