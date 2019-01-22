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
#include "Gwen/Controls/Label.h"
#include "Gwen/Utility.h"

#if USE_SDL_FONT
#include "Gwen/Fonts/SDL_Font.h"
#else
#include "Gwen/Fonts/STB_Font.h"
#endif

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( Label )
{
	m_CreatedFont = NULL;
	m_Text = new ControlsInternal::Text( this );
	m_Text->SetFont( GetSkin()->GetDefaultFont() );
	SetMouseInputEnabled( false );
	SetBounds( 0, 0, 100, 10 );
	SetAlignment( Gwen::Pos::Left | Gwen::Pos::Top );
}

void Label::PreDelete( Gwen::Skin::Base* skin )
{
	if ( m_CreatedFont )
	{
		skin->ReleaseFont( m_CreatedFont );
		delete m_CreatedFont;
		m_CreatedFont = NULL;
		SetFont( NULL );
	}
}

void Label::PostLayout( Skin::Base* /*skin*/ )
{
	m_Text->Position( m_iAlign );
}

void Label::SetAlignment( int iAlign )
{
	if ( m_iAlign == iAlign ) { return; }

	m_iAlign = iAlign;
	Invalidate();
}

int Label::GetAlignment()
{
	return m_iAlign;
}

void Label::SetText( const TextObject & str, bool bDoEvents )
{
	if ( m_Text->GetText() == str.GetUnicode() ) { return; }

	m_Text->SetString( str );
	Redraw();

	if ( bDoEvents )
	{ OnTextChanged(); }
}

void Label::SizeToContents()
{
	m_Text->SetPos( m_Padding.left, m_Padding.top );
	m_Text->RefreshSize();
	SetSize( m_Text->Width() + m_Padding.left + m_Padding.right, m_Text->Height() + m_Padding.top + m_Padding.bottom );
}

Gwen::Rect Label::GetCharacterPosition( int iChar )
{
	Gwen::Rect p = m_Text->GetCharacterPosition( iChar );
	p.x += m_Text->X();
	p.y += m_Text->Y();
	return p;
}

void Label::OnBoundsChanged( Gwen::Rect oldChildBounds )
{
	BaseClass::OnBoundsChanged( oldChildBounds );

	if ( m_Text->Wrap() )
	{
		m_Text->RefreshSize();
		Invalidate();
	}
}

void Label::SetFont( Gwen::UnicodeString strFacename, int iSize, bool bBold )
{
	if ( m_CreatedFont )
	{
		GetSkin()->ReleaseFont( m_CreatedFont );
		delete m_CreatedFont;
		m_CreatedFont = NULL;
		SetFont( NULL );
	}

#if USE_SDL_FONT
	m_CreatedFont = new Gwen::Font::SDLFont();
#else
	m_CreatedFont = new Gwen::Font::STBFont();
#endif
	Debug::AssertCheck( m_CreatedFont != NULL, "Couldn't Create Font!" );
	m_CreatedFont->SetBold(bBold);
	m_CreatedFont->SetFaceName(strFacename);
	m_CreatedFont->SetSize(iSize);
	SetFont( m_CreatedFont );
	m_Text->RefreshSize();
}