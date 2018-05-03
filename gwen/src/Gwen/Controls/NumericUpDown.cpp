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
#include "Gwen/Utility.h"
#include "Gwen/Skin.h"
#include "Gwen/Controls/NumericUpDown.h"

using namespace Gwen;
using namespace Gwen::Controls;


GWEN_CONTROL_CONSTRUCTOR( NumericUpDown )
{
	SetSize( 100, 20 );
	Controls::Base* pSplitter = new Controls::Base( this );
	pSplitter->Dock( Pos::Right );
	pSplitter->SetWidth( 13 );
	NumericUpDownButton_Up* pButtonUp = new NumericUpDownButton_Up( pSplitter );
	pButtonUp->onPress.Add( this, &NumericUpDown::OnButtonUp );
	pButtonUp->SetTabable( false );
	pButtonUp->Dock( Pos::Top );
	pButtonUp->SetHeight( 10 );
	NumericUpDownButton_Down* pButtonDown = new NumericUpDownButton_Down( pSplitter );
	pButtonDown->onPress.Add( this, &NumericUpDown::OnButtonDown );
	pButtonDown->SetTabable( false );
	pButtonDown->Dock( Pos::Fill );
	pButtonUp->SetPadding( Padding( 0, 1, 1, 0 ) );
	m_iMax = 100;
	m_iMin = 0;
	m_iNumber = 0;
	SetText( "0" );
}

void NumericUpDown::OnButtonUp( Base* /*control*/ )
{
	SyncNumberFromText();
	SetValue( m_iNumber + 1 );
}

void NumericUpDown::OnButtonDown( Base* /*control*/ )
{
	SyncNumberFromText();
	SetValue( m_iNumber - 1 );
}


void NumericUpDown::SyncTextFromNumber()
{
	SetText( Utility::ToString( m_iNumber ) );
}

void NumericUpDown::SyncNumberFromText()
{
	SetValue( ( int ) GetFloatFromText() );
}

void NumericUpDown::SetMin( int i )
{
	m_iMin = i;
}

void NumericUpDown::SetMax( int i )
{
	m_iMax = i;
}

void NumericUpDown::SetValue( int i )
{
	if ( i > m_iMax ) { i = m_iMax; }

	if ( i < m_iMin ) { i = m_iMin; }

	if ( m_iNumber == i )
	{
		return;
	}

	m_iNumber = i;
	// Don't update the text if we're typing in it..
	// Undone - any reason why not?
	//if ( !HasFocus() )
	{
		SyncTextFromNumber();
	}
	OnChange();
}

void NumericUpDown::OnChange()
{
	onChanged.Call( this );
}

void NumericUpDown::OnTextChanged()
{
	BaseClass::OnTextChanged();
	SyncNumberFromText();
}

void NumericUpDown::OnEnter()
{
	SyncNumberFromText();
	SyncTextFromNumber();
	BaseClass::OnEnter();
}