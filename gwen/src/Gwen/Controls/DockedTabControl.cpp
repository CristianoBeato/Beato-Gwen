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
#include "Gwen/Controls/DockedTabControl.h"
#include "Gwen/Controls/Highlight.h"
#include "Gwen/DragAndDrop.h"
#include "Gwen/Controls/WindowControl.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( DockedTabControl )
{
	m_WindowControl = NULL;
	Dock( Pos::Fill );
	m_pTitleBar = new TabTitleBar( this );
	m_pTitleBar->Dock( Pos::Top );
	m_pTitleBar->SetHidden( true );
	SetAllowReorder( true );
}

void DockedTabControl::Layout( Skin::Base* skin )
{
	GetTabStrip()->SetHidden( TabCount() <= 1 );
	UpdateTitleBar();
	BaseClass::Layout( skin );
}

void DockedTabControl::UpdateTitleBar()
{
	if ( !GetCurrentButton() ) { return; }

	m_pTitleBar->UpdateFromTab( GetCurrentButton() );
}

void DockedTabControl::DragAndDrop_StartDragging( Gwen::DragAndDrop::Package* pPackage, int x, int y )
{
	BaseClass::DragAndDrop_StartDragging( pPackage, x, y );
	SetHidden( true );
	// This hiding our parent thing is kind of lousy.
	GetParent()->SetHidden( true );
}

void DockedTabControl::DragAndDrop_EndDragging( bool bSuccess, int /*x*/, int /*y*/ )
{
	SetHidden( false );

	if ( !bSuccess )
	{
		GetParent()->SetHidden( false );
	}

	/*
		if ( !bSuccess )
		{
			// Create our window control
			if ( !m_WindowControl )
			{
				m_WindowControl = new WindowControl( GetCanvas() );
				m_WindowControl->SetBounds( x, y, Width(), Height() );
			}

			m_WindowControl->SetPosition( x, y );
			SetParent( m_WindowControl );
			SetPosition( 0, 0 );
			Dock( Pos::Fill );
		}
		*/
}

void DockedTabControl::MoveTabsTo( DockedTabControl* pTarget )
{
	Base::List Children = GetTabStrip()->Children;

	for ( Base::List::iterator iter = Children.begin(); iter != Children.end(); ++iter )
	{
		TabButton* pButton = gwen_cast<TabButton> ( *iter );

		if ( !pButton ) { continue; }

		pTarget->AddPage( pButton );
	}

	Invalidate();
}