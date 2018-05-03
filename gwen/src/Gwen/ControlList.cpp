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
#include "Gwen/ControlList.h"

using namespace Gwen;
using namespace Gwen::Controls;

void ControlList::Enable()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->SetDisabled( false );
	}
}

void ControlList::Disable()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->SetDisabled( true );
	}
}

void ControlList::Show()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->Show();
	}
}

void ControlList::Hide()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->Hide();
	}
}

Gwen::TextObject ControlList::GetValue()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		return ( *it )->GetValue();
	}

	return "";
}

void ControlList::SetValue( const Gwen::TextObject & value )
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->SetValue( value );
	}
}

void ControlList::MoveBy( const Gwen::Point & point )
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->MoveBy( point.x, point.y );
	}
}

void ControlList::DoAction()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->DoAction();
	}
}

void ControlList::SetActionInternal( Gwen::Event::Handler* pObject, void ( Gwen::Event::Handler::*f )( Gwen::Event::Info ), const Gwen::Event::Packet & packet )
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		( *it )->SetAction( pObject, f, packet );
	}
}