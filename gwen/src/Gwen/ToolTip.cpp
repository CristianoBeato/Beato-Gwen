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

#include "Gwen/ToolTip.h"

using namespace Gwen;
using namespace Gwen::Controls;

namespace ToolTip
{
	Base* g_ToolTip = NULL;

	GWEN_EXPORT bool TooltipActive()
	{
		return g_ToolTip != NULL;
	}

	void Enable( Controls::Base* pControl )
	{
		if ( !pControl->GetToolTip() )
		{ return; }

		g_ToolTip = pControl;
	}

	void Disable( Controls::Base* pControl )
	{
		if ( g_ToolTip == pControl )
		{
			g_ToolTip = NULL;
		}
	}

	void RenderToolTip( Skin::Base* skin )
	{
		if ( !g_ToolTip ) { return; }

		Gwen::Renderer::BaseRender* render = skin->GetRender();
		Gwen::Point pOldRenderOffset = render->GetRenderOffset();
		Gwen::Point MousePos = Input::GetMousePosition();
		Gwen::Rect Bounds = g_ToolTip->GetToolTip()->GetBounds();
		Gwen::Rect rOffset = Gwen::Rect( MousePos.x - Bounds.w * 0.5f, MousePos.y - Bounds.h - 10, Bounds.w, Bounds.h );
		rOffset = Utility::ClampRectToRect( rOffset, g_ToolTip->GetCanvas()->GetBounds() );
		//Calculate offset on screen bounds
		render->AddRenderOffset( rOffset );
		render->EndClip();
		skin->DrawToolTip( g_ToolTip->GetToolTip() );
		g_ToolTip->GetToolTip()->DoRender( skin );
		render->SetRenderOffset( pOldRenderOffset );
	}

	void ControlDeleted( Controls::Base* pControl )
	{
		Disable( pControl );
	}
}
