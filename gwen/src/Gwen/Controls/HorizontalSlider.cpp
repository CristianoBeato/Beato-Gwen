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

#include "Gwen/Controls/Slider.h"
#include "Gwen/Controls/HorizontalSlider.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::ControlsInternal;


GWEN_CONTROL_CONSTRUCTOR( HorizontalSlider )
{
	m_SliderBar->SetHorizontal( true );
}

float HorizontalSlider::CalculateValue()
{
	return ( float ) m_SliderBar->X() / ( float )( Width() - m_SliderBar->Width() );
}

void HorizontalSlider::UpdateBarFromValue()
{
	m_SliderBar->MoveTo( ( Width() - m_SliderBar->Width() ) * ( m_fValue ), m_SliderBar->Y() );
}

void HorizontalSlider::OnMouseClickLeft( int x, int y, bool bDown )
{
	m_SliderBar->MoveTo( CanvasPosToLocal( Gwen::Point( x, y ) ).x - m_SliderBar->Width() * 0.5,  m_SliderBar->Y() );
	m_SliderBar->OnMouseClickLeft( x, y, bDown );
	OnMoved( m_SliderBar );
}

void HorizontalSlider::Layout( Skin::Base* /*skin*/ )
{
	m_SliderBar->SetSize( 15, Height() );
}

void HorizontalSlider::Render( Skin::Base* skin )
{
	skin->DrawSlider( this, true, m_bClampToNotches ? m_iNumNotches : 0, m_SliderBar->Width() );
}