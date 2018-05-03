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

#include "Gwen/Controls/RadioButtonController.h"
#include "Gwen/Controls/RadioButton.h"
#include "Gwen/Utility.h"

using namespace Gwen;
using namespace Gwen::Controls;


GWEN_CONTROL_CONSTRUCTOR( RadioButtonController )
{
	m_Selected = NULL;
	SetTabable( false );
	SetKeyboardInputEnabled( false );
}

void RadioButtonController::OnRadioClicked( Gwen::Controls::Base* pFromPanel )
{
	RadioButton* pCheckedRadioButton = gwen_cast<RadioButton> ( pFromPanel );

	//Iterate through all other buttons and set them to false;
	for ( Base::List::iterator iter = Children.begin(); iter != Children.end(); ++iter )
	{
		Base* pChild = *iter;
		LabeledRadioButton* pLRB = gwen_cast<LabeledRadioButton> ( pChild );

		if ( pLRB )
		{
			RadioButton* pChildRadioButton = pLRB->GetRadioButton();

			if ( pChildRadioButton == pCheckedRadioButton )
			{
				m_Selected = pLRB;
			}
			else
			{
				pLRB->GetRadioButton()->SetChecked( false );
			}
		}
	}

	OnChange();
}

void RadioButtonController::OnChange()
{
	onSelectionChange.Call( this );
}

LabeledRadioButton* RadioButtonController::AddOption( const Gwen::String & strText, const Gwen::String & strOptionName )
{
	return AddOption( Gwen::Utility::StringToUnicode( strText ), strOptionName );
}

LabeledRadioButton* RadioButtonController::AddOption( const Gwen::UnicodeString & strText, const Gwen::String & strOptionName )
{
	LabeledRadioButton* lrb = new LabeledRadioButton( this );
	lrb->SetName( strOptionName );
	lrb->GetLabel()->SetText( strText );
	lrb->GetRadioButton()->onChecked.Add( this, &RadioButtonController::OnRadioClicked );
	lrb->Dock( Pos::Top );
	lrb->SetMargin( Margin( 0, 1, 0, 1 ) );
	lrb->SetKeyboardInputEnabled( false );
	lrb->SetTabable( false );
	Invalidate();
	return lrb;
}