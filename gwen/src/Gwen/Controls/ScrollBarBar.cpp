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

#include "Gwen/Controls/ScrollBar.h"
#include "Gwen/Controls/ScrollBarBar.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::ControlsInternal;

//Actual bar representing height of parent

GWEN_CONTROL_CONSTRUCTOR( ScrollBarBar )
{
	RestrictToParent( true );
	SetTarget( this );
}

void ScrollBarBar::Render( Skin::Base* skin )
{
	skin->DrawScrollBarBar( this, m_bDepressed, IsHovered(), m_bHorizontal );
	BaseClass::Render( skin );
}

void ScrollBarBar::OnMouseMoved( int x, int y, int deltaX, int deltaY )
{
	BaseClass::OnMouseMoved( x, y, deltaX, deltaY );

	if ( !m_bDepressed )
	{ return; }

	InvalidateParent();
}

void ScrollBarBar::OnMouseClickLeft( int x, int y, bool bDown )
{
	BaseClass::OnMouseClickLeft( x, y, bDown );
	InvalidateParent();
}

void ScrollBarBar::Layout( Skin::Base* /*skin*/ )
{
	if ( !GetParent() )
	{ return; }

	//Move to our current position to force clamping - is this a hack?
	MoveTo( X(), Y() );
}

void ScrollBarBar::MoveTo( int x, int y )
{
	BaseClass::MoveTo( x, y );
}