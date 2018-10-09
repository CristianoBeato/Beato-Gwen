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

#include "Gwen/Gwen.h"
#include <math.h>


/*
	Here we're drawing a few symbols such as the directional arrows and the checkbox check
	Texture'd skins don't generally use these - but the Simple skin does. We did originally
	use the marlett font to draw these.. but since that's a Windows font it wasn't a very
	good cross platform solution.
*/

bool Gwen::Skin::Base::Init(const TextObject & TextureName) { return false; }

void Gwen::Skin::Base::Clear(void) { };

void Gwen::Skin::Base::DrawArrowDown(Gwen::Rect rect)
{
	float x = (rect.w / 5.0f);
	float y = (rect.h / 5.0f);
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 0.0f, rect.y + y * 1.0f, x, y * 1.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 1.0f, x, y * 2.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 2.0f, rect.y + y * 1.0f, x, y * 3.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 3.0f, rect.y + y * 1.0f, x, y * 2.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 4.0f, rect.y + y * 1.0f, x, y * 1.0f));
}

void Gwen::Skin::Base::DrawArrowUp(Gwen::Rect rect)
{
	float x = (rect.w / 5.0f);
	float y = (rect.h / 5.0f);
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 0.0f, rect.y + y * 3.0f, x, y * 1.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 2.0f, x, y * 2.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 2.0f, rect.y + y * 1.0f, x, y * 3.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 3.0f, rect.y + y * 2.0f, x, y * 2.0f));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 4.0f, rect.y + y * 3.0f, x, y * 1.0f));
}

void Gwen::Skin::Base::DrawArrowLeft(Gwen::Rect rect)
{
	float x = (rect.w / 5.0f);
	float y = (rect.h / 5.0f);
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 3.0f, rect.y + y * 0.0f, x * 1.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 2.0f, rect.y + y * 1.0f, x * 2.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 2.0f, x * 3.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 2.0f, rect.y + y * 3.0f, x * 2.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 3.0f, rect.y + y * 4.0f, x * 1.0f, y));
}

void Gwen::Skin::Base::DrawArrowRight(Gwen::Rect rect)
{
	float x = (rect.w / 5.0f);
	float y = (rect.h / 5.0f);
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 0.0f, x * 1.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 1.0f, x * 2.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 2.0f, x * 3.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 3.0f, x * 2.0f, y));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 4.0f, x * 1.0f, y));
}

void Gwen::Skin::Base::DrawCheck(Gwen::Rect rect)
{
	float x = (rect.w / 5.0f);
	float y = (rect.h / 5.0f);
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 0.0f, rect.y + y * 3.0f, x * 2, y * 2));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 1.0f, rect.y + y * 4.0f, x * 2, y * 2));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 2.0f, rect.y + y * 3.0f, x * 2, y * 2));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 3.0f, rect.y + y * 1.0f, x * 2, y * 2));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + x * 4.0f, rect.y + y * 0.0f, x * 2, y * 2));
}

void Gwen::Skin::Base::DrawTreeNode(Controls::Base* ctrl, bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot)
{
	GetRender()->SetDrawColor(Colors.Tree.Lines);

	if (!bIsRoot)
		GetRender()->DrawFilledRect(Gwen::Rect(8, iHalfWay, 16 - 9, 1));

	if (!bOpen)
		return;

	GetRender()->DrawFilledRect(Gwen::Rect(14 + 7, iLabelHeight + 1, 1, iLastBranch + iHalfWay - iLabelHeight));
}

void Gwen::Skin::Base::DrawPropertyTreeNode(Controls::Base* control, int BorderLeft, int BorderTop)
{
	Gwen::Rect rect = control->GetRenderBounds();
	m_Render->SetDrawColor(Colors.Properties.Border);
	m_Render->DrawFilledRect(Gwen::Rect(rect.x, rect.y, BorderLeft, rect.h));
	m_Render->DrawFilledRect(Gwen::Rect(rect.x + BorderLeft, rect.y, rect.w - BorderLeft, BorderTop));
}

void Gwen::Skin::Base::DrawPropertyRow(Controls::Base* control, int iWidth, bool bBeingEdited, bool bHovered)
{
	Gwen::Rect rect = control->GetRenderBounds();

	if (bBeingEdited) { m_Render->SetDrawColor(Colors.Properties.Column_Selected); }
	else if (bHovered) { m_Render->SetDrawColor(Colors.Properties.Column_Hover); }
	else { m_Render->SetDrawColor(Colors.Properties.Column_Normal); }

	m_Render->DrawFilledRect(Gwen::Rect(0, rect.y, iWidth, rect.h));

	if (bBeingEdited) { m_Render->SetDrawColor(Colors.Properties.Line_Selected); }
	else if (bHovered) { m_Render->SetDrawColor(Colors.Properties.Line_Hover); }
	else { m_Render->SetDrawColor(Colors.Properties.Line_Normal); }

	m_Render->DrawFilledRect(Gwen::Rect(iWidth, rect.y, 1, rect.h));
	rect.y += rect.h - 1;
	rect.h = 1;
	m_Render->DrawFilledRect(rect);
}

Gwen::Skin::Base::Base(Gwen::Renderer::Base * renderer)
{
	m_DefaultFont.facename = L"Arial";
	m_DefaultFont.size = 10.0f;
	m_Render = renderer;
}

Gwen::Skin::Base::~Base(void)
{
	ReleaseFont(&m_DefaultFont);
}

void Gwen::Skin::Base::ReleaseFont(Gwen::Font * fnt)
{
	if (!fnt)
		return;

	if (!m_Render)
		return;

	m_Render->FreeFont(fnt);
}