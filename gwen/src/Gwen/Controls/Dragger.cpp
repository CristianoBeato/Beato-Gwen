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

#include "Gwen/Controls/Dragger.h"

using namespace Gwen;
using namespace Gwen::ControlsInternal;



GWEN_CONTROL_CONSTRUCTOR( Dragger )
{
	m_pTarget = NULL;
	SetMouseInputEnabled( true );
	m_bDepressed = false;
	m_bDoMove = true;
}

void Dragger::OnMouseClickLeft( int x, int y, bool bDown )
{
	if ( bDown )
	{
		m_bDepressed = true;

		if ( m_pTarget )
		{ m_HoldPos = m_pTarget->CanvasPosToLocal( Gwen::Point( x, y ) ); }

		Gwen::MouseFocus = this;
		onDragStart.Call( this );
	}
	else
	{
		m_bDepressed = false;
		Gwen::MouseFocus = NULL;
	}
}

void Dragger::OnMouseMoved( int x, int y, int deltaX, int deltaY )
{
	if ( !m_bDepressed ) { return; }

	if ( m_bDoMove && m_pTarget )
	{
		Gwen::Point p = Gwen::Point( x - m_HoldPos.x, y - m_HoldPos.y );

		// Translate to parent
		if ( m_pTarget->GetParent() )
		{ p = m_pTarget->GetParent()->CanvasPosToLocal( p ); }

		m_pTarget->MoveTo( p.x, p.y );
	}

	Gwen::Event::Information info;
	info.Point = Gwen::Point( deltaX, deltaY );
	onDragged.Call( this, info );
}

void Dragger::Render( Skin::Base* skin )
{
	//skin->DrawButton(this,false,false, false);
}

void Dragger::OnMouseDoubleClickLeft( int x, int y )
{
	onDoubleClickLeft.Call( this );
}