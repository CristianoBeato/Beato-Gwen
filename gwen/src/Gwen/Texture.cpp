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

#include "Texture.h"

Gwen::Texture::Texture(void)
{
	data = NULL;
	width = 4;
	height = 4;
	failed = false;
}

void Gwen::Texture::Load(const TextObject & str, Gwen::Renderer::Base * render)
{
	name = str;
	Gwen::Debug::AssertCheck(render != NULL, "No renderer!");
	render->LoadTexture(this);
}

void Gwen::Texture::Release(Gwen::Renderer::Base * render)
{
	render->FreeTexture(this);
	data = NULL;
	width = 0;
	height = 0;
}

bool Gwen::Texture::FailedToLoad(void) const
{
	return failed;
}

void Gwen::Texture::GenImage(Gwen::Renderer::Base * render, const byte * src, int width, int height)
{
}

TextObject Gwen::Texture::getName(void) const
{
	return name;
}

TextObject &Gwen::Texture::getName(void)
{
	return name;
}

int Gwen::Texture::getWidth(void) const
{
	return width;
}

int & Gwen::Texture::getWidth(void)
{
	return width;
}

int Gwen::Texture::getHeight(void) const
{
	return height;
}

int & Gwen::Texture::getHeight(void)
{
	return height;
}

void * Gwen::Texture::getData(void) const
{
	return data;
}

void *& Gwen::Texture::getData(void)
{
	return this->data;
}
