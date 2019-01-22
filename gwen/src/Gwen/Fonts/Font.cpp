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

#include "Gwen/Fonts/Font.h"

Gwen::Font::FontBase::FontBase(void)
{
	m_data = NULL;
	m_facename = L"Arial";
	m_size = 10;
	m_realsize = 0;
	m_bold = false;
	m_AtlasWidth = 0;
	m_AtlasHeight = 0;
	m_AtlasDepth = 0;
	m_AtlasBuff =nullptr;
}

Gwen::Font::FontBase::~FontBase(void)
{
}

void Gwen::Font::FontBase::SetFaceName(const UnicodeString name)
{
	m_facename = name;
}

void Gwen::Font::FontBase::SetSize(const float size)
{
	m_size = size;
}

void Gwen::Font::FontBase::SetBold(const bool bold)
{
	m_bold = bold;
}

UnicodeString & Gwen::Font::FontBase::FaceName(void)
{
	return m_facename;
}

const UnicodeString Gwen::Font::FontBase::FaceName(void) const
{
	return m_facename;
}

void *& Gwen::Font::FontBase::GetData(void)
{
	return m_data;
}

const float Gwen::Font::FontBase::Size(void) const
{
	return m_size;
}

float &Gwen::Font::FontBase::Size(void)
{
	return m_size;
}

Uint16 Gwen::Font::FontBase::GetAtlasWidth(void) const
{
	return m_AtlasWidth;
}

Uint16 Gwen::Font::FontBase::GetAtlasHeight(void) const
{
	return m_AtlasHeight;
}

Uint8 Gwen::Font::FontBase::GetAtlasColorDept(void) const
{
	return m_AtlasDepth;
}

byte * Gwen::Font::FontBase::GetAtlasBuff(void) const
{
	return m_AtlasBuff;
}
