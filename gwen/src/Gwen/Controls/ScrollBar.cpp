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
#include "Gwen/Controls/ScrollBarButton.h"
#include "Gwen/Controls/ScrollBarBar.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::ControlsInternal;


GWEN_CONTROL_CONSTRUCTOR( BaseScrollBar )
{
	for ( int i = 0; i < 2; i++ )
	{
		m_ScrollButton[i] = new ScrollBarButton( this );
	}

	m_Bar = new ScrollBarBar( this );
	SetBounds( 0, 0, 15, 15 );
	m_bDepressed = false;
	m_fScrolledAmount = 0;
	m_fContentSize = 0;
	m_fViewableContentSize = 0;
	SetNudgeAmount( 20 );
}

void BaseScrollBar::Render( Skin::Base* skin )
{
	skin->DrawScrollBar( this, IsHorizontal(), m_bDepressed );
}

void BaseScrollBar::OnBarMoved( Controls::Base* /*control*/ )
{
	onBarMoved.Call( this );
}

void BaseScrollBar::BarMovedNotification()
{
	OnBarMoved( this );
}

void BaseScrollBar::SetContentSize( float size )
{
	if ( m_fContentSize != size )
	{
		Invalidate();
	}

	m_fContentSize = size;
}
void BaseScrollBar::SetViewableContentSize( float size )
{
	if ( m_fViewableContentSize != size )
	{ Invalidate(); }

	m_fViewableContentSize = size;
}

bool BaseScrollBar::SetScrolledAmount( float amount, bool forceUpdate )
{
	if ( m_fScrolledAmount == amount && !forceUpdate ) { return false; }

	m_fScrolledAmount = amount;
	Invalidate();
	BarMovedNotification();
	return true;
}






