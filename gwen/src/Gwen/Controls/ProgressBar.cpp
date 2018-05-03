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

#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/Controls/ProgressBar.h"
#include "Gwen/Anim.h"
#include "Gwen/Utility.h"

using namespace Gwen;
using namespace Gwen::Controls;

class ProgressBarThink : public Gwen::Anim::Animation
{
	public:

		ProgressBarThink()
		{
			m_fLastFrame = 0.0f;
		}

		virtual void Think()
		{
			float fDiff = Platform::GetTimeInSeconds() - m_fLastFrame;
			gwen_cast<ProgressBar> ( m_Control )->CycleThink( Gwen::Clamp( fDiff, 0.f, 0.3f ) );
			m_fLastFrame = Platform::GetTimeInSeconds();
		}


		float	m_fLastFrame;
};


GWEN_CONTROL_CONSTRUCTOR( ProgressBar )
{
	SetMouseInputEnabled( true );
	SetBounds( Gwen::Rect( 0, 0, 128, 32 ) );
	SetTextPadding( Padding( 3, 3, 3, 3 ) );
	SetHorizontal();
	SetAlignment( Gwen::Pos::Center );
	m_fProgress = 0.0f;
	m_bAutoLabel = true;
	m_fCycleSpeed = 0.0f;
	Gwen::Anim::Add( this, new ProgressBarThink() );
}

void ProgressBar::SetValue( float val )
{
	if ( val < 0 )
	{ val = 0; }

	if ( val > 1 )
	{ val = 1; }

	m_fProgress = val;

	if ( m_bAutoLabel )
	{
		int displayVal = m_fProgress * 100;
		SetText( Utility::ToString( displayVal ) + "%" );
	}
}

void ProgressBar::CycleThink( float fDelta )
{
	if ( !Visible() ) { return; }

	if ( m_fCycleSpeed == 0.0f ) { return; }

	m_fProgress += m_fCycleSpeed * fDelta;

	if ( m_fProgress < 0.0f ) { m_fProgress += 1.0f; }

	if ( m_fProgress > 1.0f ) { m_fProgress -= 1.0f; }

	Redraw();
}

void ProgressBar::Render( Skin::Base* skin )
{
	skin->DrawProgressBar( this, m_bHorizontal, m_fProgress );
}

float ProgressBar::GetCycleSpeed()
{
	return m_fCycleSpeed;
}

void ProgressBar::SetCycleSpeed( float f )
{
	m_fCycleSpeed = f;
}