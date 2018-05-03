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

#include "Gwen/Controls/ImagePanel.h"
#include "Gwen/Controls/Label.h"
#include "Gwen/Controls/ResizableControl.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::ControlsInternal;

GWEN_CONTROL_CONSTRUCTOR( ResizableControl )
{
	m_bResizable = true;
	m_MinimumSize = Gwen::Point( 5, 5 );
	m_bClampMovement = false;
	m_Resizer[5] = NULL;
	m_Resizer[0] = NULL;
	m_Resizer[2] = new Resizer( this );
	m_Resizer[2]->Dock( Pos::Bottom );
	m_Resizer[2]->SetResizeDir( Pos::Bottom );
	m_Resizer[2]->SetTarget( this );
	m_Resizer[2]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[1] = new Resizer( m_Resizer[2] );
	m_Resizer[1]->Dock( Pos::Left );
	m_Resizer[1]->SetResizeDir( Pos::Bottom | Pos::Left );
	m_Resizer[1]->SetTarget( this );
	m_Resizer[1]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[3] = new Resizer( m_Resizer[2] );
	m_Resizer[3]->Dock( Pos::Right );
	m_Resizer[3]->SetResizeDir( Pos::Bottom | Pos::Right );
	m_Resizer[3]->SetTarget( this );
	m_Resizer[3]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[8] = new Resizer( this );
	m_Resizer[8]->Dock( Pos::Top );
	m_Resizer[8]->SetResizeDir( Pos::Top );
	m_Resizer[8]->SetTarget( this );
	m_Resizer[8]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[7] = new Resizer( m_Resizer[8] );
	m_Resizer[7]->Dock( Pos::Left );
	m_Resizer[7]->SetResizeDir( Pos::Top | Pos::Left );
	m_Resizer[7]->SetTarget( this );
	m_Resizer[7]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[9] = new Resizer( m_Resizer[8] );
	m_Resizer[9]->Dock( Pos::Right );
	m_Resizer[9]->SetResizeDir( Pos::Top | Pos::Right );
	m_Resizer[9]->SetTarget( this );
	m_Resizer[9]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[4] = new Resizer( this );
	m_Resizer[4]->Dock( Pos::Left );
	m_Resizer[4]->SetResizeDir( Pos::Left );
	m_Resizer[4]->SetTarget( this );
	m_Resizer[4]->onResize.Add( this, &ResizableControl::OnResizedInternal );
	m_Resizer[6] = new Resizer( this );
	m_Resizer[6]->Dock( Pos::Right );
	m_Resizer[6]->SetResizeDir( Pos::Right );
	m_Resizer[6]->SetTarget( this );
	m_Resizer[6]->onResize.Add( this, &ResizableControl::OnResizedInternal );
}

void ResizableControl::DisableResizing()
{
	for ( Base::List::iterator it = Children.begin(); it != Children.end(); ++it )
	{
		Resizer* resizer = gwen_cast<Resizer> ( *it );

		if ( !resizer ) { continue; }

		resizer->SetMouseInputEnabled( false );
        //resizer->SetHidden( false );
		//SetPadding( Padding( resizer->Width(), resizer->Width(), resizer->Width(), resizer->Width() ) );
	}
}

bool ResizableControl::SetBounds( int x, int y, int w, int h )
{
	Gwen::Point minSize = GetMinimumSize();

	// Clamp Minimum Size
	if ( w < minSize.x ) { w = minSize.x; }

	if ( h < minSize.y ) { h = minSize.y; }

	// Clamp to parent's window
	Base* pParent = GetParent();

	if ( pParent && m_bClampMovement )
	{
		if ( x + w > pParent->Width() ) { x = pParent->Width() - w; }

		if ( x < 0 ) { x = 0; }

		if ( y + h > pParent->Height() ) { y = pParent->Height() - h; }

		if ( y < 0 ) { y = 0; }
	}

	return BaseClass::SetBounds( x, y, w, h );
}

void ResizableControl::OnResizedInternal( Controls::Base* /*pControl*/ )
{
	onResize.Call( this );
	OnResized();
}
