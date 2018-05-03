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
#include "Gwen/Controls/VerticalScrollBar.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( VerticalScrollBar )
{
	m_Bar->SetVertical();
	m_ScrollButton[SCROLL_BUTTON_UP]->SetDirectionUp();
	m_ScrollButton[SCROLL_BUTTON_UP]->onPress.Add( this, &VerticalScrollBar::NudgeUp );
	m_ScrollButton[SCROLL_BUTTON_DOWN]->SetDirectionDown();
	m_ScrollButton[SCROLL_BUTTON_DOWN]->onPress.Add( this, &VerticalScrollBar::NudgeDown );
	m_Bar->onDragged.Add( this, &VerticalScrollBar::OnBarMoved );
}

void VerticalScrollBar::Layout( Skin::Base* skin )
{
	BaseClass::Layout( skin );
	m_ScrollButton[SCROLL_BUTTON_UP]->Dock( Pos::Top );
	m_ScrollButton[SCROLL_BUTTON_UP]->SetHeight( Width() );
	m_ScrollButton[SCROLL_BUTTON_DOWN]->Dock( Pos::Bottom );
	m_ScrollButton[SCROLL_BUTTON_DOWN]->SetHeight( Width() );
	m_Bar->SetWidth( GetButtonSize() );
	//Add padding
	m_Bar->SetPadding( Padding( 0, GetButtonSize(), 0, GetButtonSize() ) );
	//Calculate bar sizes
	float barHeight = ( m_fViewableContentSize / m_fContentSize ) * ( Height() - GetButtonSize() );

	if ( barHeight < GetButtonSize() * 0.5 )
	{ barHeight = GetButtonSize() * 0.5; }

	m_Bar->SetHeight( barHeight );
	m_Bar->SetHidden( Height() - ( GetButtonSize() * 2 ) <= barHeight );

	if ( Hidden() )
	{ SetScrolledAmount( 0, true ); }

	//Based on our last scroll amount, produce a position for the bar
	if ( !m_Bar->IsDepressed() )
	{
		SetScrolledAmount( GetScrolledAmount(), true );
	}
}

void VerticalScrollBar::ScrollToTop()
{
	SetScrolledAmount( 0, true );
}
void VerticalScrollBar::ScrollToBottom()
{
	SetScrolledAmount( 1, true );
}
void VerticalScrollBar::NudgeUp( Base* /*control*/ )
{
	if ( !IsDisabled() )
	{ SetScrolledAmount( GetScrolledAmount() - GetNudgeAmount(), true ); }
}

void VerticalScrollBar::NudgeDown( Base* /*control*/ )
{
	if ( !IsDisabled() )
	{ SetScrolledAmount( GetScrolledAmount() + GetNudgeAmount(), true ); }
}

float VerticalScrollBar::GetNudgeAmount()
{
	if ( m_bDepressed )
	{ return m_fViewableContentSize / m_fContentSize; }
	else
	{ return BaseClass::GetNudgeAmount(); }
}

void VerticalScrollBar::OnMouseClickLeft( int x, int y, bool bDown )
{
	if ( bDown )
	{
		m_bDepressed = true;
		Gwen::MouseFocus = this;
	}
	else
	{
		Gwen::Point clickPos = CanvasPosToLocal( Gwen::Point( x, y ) );

		if ( clickPos.y < m_Bar->Y() )
		{ NudgeUp( this ); }
		else if ( clickPos.y > m_Bar->Y() + m_Bar->Height() )
		{ NudgeDown( this ); }

		m_bDepressed = false;
		Gwen::MouseFocus = NULL;
	}
}

float VerticalScrollBar::CalculateScrolledAmount()
{
	return ( float )( m_Bar->Y() - GetButtonSize() ) / ( float )( Height() - m_Bar->Height() - ( GetButtonSize() * 2 ) );
}

bool VerticalScrollBar::SetScrolledAmount( float amount, bool forceUpdate )
{
	amount = Gwen::Clamp( amount, 0.f, 1.f );

	if ( !BaseClass::SetScrolledAmount( amount, forceUpdate ) )
	{ return false; }

	if ( forceUpdate )
	{
		int newY = GetButtonSize() + ( amount * ( ( Height() - m_Bar->Height() ) - ( GetButtonSize() * 2 ) ) );
		m_Bar->MoveTo( m_Bar->X(), newY );
	}

	return true;
}

void VerticalScrollBar::OnBarMoved( Controls::Base* control )
{
	if ( m_Bar->IsDepressed() )
	{
		SetScrolledAmount( CalculateScrolledAmount(), false );
		BaseClass::OnBarMoved( control );
	}
	else
	{
		InvalidateParent();
	}
}