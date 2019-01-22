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
#include "Gwen/Controls/WindowCanvas.h"
#include "Gwen/Controls/Menu.h"
#include "Gwen/DragAndDrop.h"
#include "Gwen/ToolTip.h"

#ifndef GWEN_NO_ANIMATION
#include "Gwen/Anim.h"
#endif

Gwen::Controls::WindowCanvas::WindowCanvas( int x, int y, int w, int h, Gwen::Skin::Base* pSkin, const Gwen::String & strWindowTitle, unsigned int flags )
	: BaseClass( NULL )
{
	m_bQuit = false;
	m_bCanMaximize = true;
	m_bIsMaximized = false;
	SetPadding( Padding( 1, 0, 1, 1 ) );
	// Centering the window on the desktop
	{
		int dw, dh;
		Gwen::Platform::GetDesktopSize( dw, dh );

		if ( x < 0 )
			x = ( dw - w ) * 0.5; 
		if ( y < 0 ) 
			y = ( dh - h ) * 0.5;
	}
	m_pOSWindow = Gwen::Platform::CreatePlatformWindow( x, y, w, h, strWindowTitle, flags);
	m_WindowPos  = Gwen::Point( x, y );
	pSkin->GetRender()->InitializeContext( this );
	pSkin->GetRender()->Init();
	m_Skin = m_pSkinChange = pSkin;
	SetSize( w, h );
}

Gwen::Controls::WindowCanvas::~WindowCanvas()
{
	DestroyWindow();
}

void* Gwen::Controls::WindowCanvas::GetWindow()
{
	return m_pOSWindow;
}

void Gwen::Controls::WindowCanvas::Layout( Skin::Base* skin )
{
	/*
	m_Sizer->BringToFront();
	m_Sizer->Position( Pos::Right | Pos::Bottom );
	*/
	BaseClass::Layout( skin );
}

void Gwen::Controls::WindowCanvas::DoThink()
{
	if (Platform::IsWindowVisible(m_pOSWindow))
		Platform::MessagePump(m_pOSWindow, this);

	BaseClass::DoThink();

	if (Platform::IsWindowVisible(m_pOSWindow))
		RenderCanvas();
}

void Gwen::Controls::WindowCanvas::RenderCanvas(void)
{
	//
	// If there isn't anything going on we sleep the thread for a few ms
	// This gives some cpu time back to the os. If you're using a rendering
	// method that needs continual updates, just call canvas->redraw every frame.
	//
	if ( !NeedsRedraw() )
	{
		Platform::Sleep( 10 );
		return;
	}

	m_bNeedsRedraw = false;
	Gwen::Renderer::BaseRender* render = m_Skin->GetRender();

	if ( render->BeginContext( this ) )
	{
		render->Begin();
		RecurseLayout( m_Skin );
		render->SetClipRegion( GetRenderBounds() );
		render->SetRenderOffset( Gwen::Point( X() * -1, Y() * -1 ) );
		render->SetScale( Scale() );

		if ( m_bDrawBackground )
		{
			render->SetDrawColor( m_BackgroundColor );
			render->DrawFilledRect( GetRenderBounds() );
		}

		DoRender( m_Skin );
		DragAndDrop::RenderOverlay( this, m_Skin );
		ToolTip::RenderToolTip( m_Skin );
		render->End();
	}

	render->EndContext( this );
	render->PresentContext( this );
}

void Gwen::Controls::WindowCanvas::Render( Skin::Base* skin )
{
	bool bHasFocus = IsOnTop();

	//skin->DrawWindow( this, 2, bHasFocus );
}

void Gwen::Controls::WindowCanvas::DestroyWindow()
{
	if ( m_pOSWindow )
	{
		GetSkin()->GetRender()->ShutdownContext( this );
		Gwen::Platform::DestroyPlatformWindow( m_pOSWindow );
		m_pOSWindow = NULL;
	}
}

bool Gwen::Controls::WindowCanvas::InputQuit()
{
	m_bQuit = true;
	return true;
}

Skin::Base* Gwen::Controls::WindowCanvas::GetSkin( void )
{
	if ( m_pSkinChange )
	{
		SetSkin( m_pSkinChange );
		m_pSkinChange = NULL;
	}

	return BaseClass::GetSkin();
}

void Gwen::Controls::WindowCanvas::Dragger_Start()
{
	Gwen::Platform::GetCursorPos( m_HoldPos );
	m_HoldPos.x -= m_WindowPos.x;
	m_HoldPos.y -= m_WindowPos.y;
}

void Gwen::Controls::WindowCanvas::Dragger_Moved()
{
	Gwen::Point p;
	Gwen::Platform::GetCursorPos( p );

	//
	// Dragged out of maximized
	if ( m_bIsMaximized )
	{
		float fOldWidth = Width();
		SetMaximize( false );
		// Change the hold pos to be the same distance across the titlebar of the resized window
		m_HoldPos.x = ( ( float ) m_HoldPos.x ) * ( ( float ) Width() / fOldWidth );
		m_HoldPos.y = 10;
	}

	SetPos( p.x - m_HoldPos.x, p.y - m_HoldPos.y );
}

void Gwen::Controls::WindowCanvas::SetPos( int x, int y )
{
	int w, h;
	Gwen::Platform::GetDesktopSize( w, h );
	y = Gwen::Clamp( y, 0, h );
	m_WindowPos.x = x;
	m_WindowPos.y = y;
	Gwen::Platform::SetBoundsPlatformWindow( m_pOSWindow, x, y, Width(), Height() );
}

bool Gwen::Controls::WindowCanvas::IsOnTop()
{
	return Gwen::Platform::HasFocusPlatformWindow( m_pOSWindow );
}

void Gwen::Controls::WindowCanvas::Sizer_Moved()
{
	Gwen::Point p;
	Gwen::Platform::GetCursorPos( p );
	int w = ( p.x ) - m_WindowPos.x;
	int h = ( p.y ) - m_WindowPos.y;
	w = Clamp( w, 100, 9999 );
	h = Clamp( h, 100, 9999 );
	Gwen::Platform::SetBoundsPlatformWindow( m_pOSWindow, m_WindowPos.x, m_WindowPos.y, w, h );
	GetSkin()->GetRender()->ResizedContext( this, w, h );
	this->SetSize( w, h );
	BaseClass::DoThink();
	RenderCanvas();
}

void Gwen::Controls::WindowCanvas::OnTitleDoubleClicked()
{
	if ( !CanMaximize() )
		return;

	SetMaximize( !m_bIsMaximized );
}

void Gwen::Controls::WindowCanvas::SetMaximize( bool b )
{
	m_bIsMaximized = b;
	Gwen::Point pSize, pPos;
	Gwen::Platform::SetWindowMaximized( m_pOSWindow, m_bIsMaximized, pPos, pSize );
	SetSize( pSize.x, pSize.y );
	m_WindowPos = pPos;
	GetSkin()->GetRender()->ResizedContext( this, pSize.x, pSize.y );
	BaseClass::DoThink();
	RenderCanvas();
}

void Gwen::Controls::WindowCanvas::SetSizable(bool b)
{
	m_Sizer->SetHidden(!b);
}

bool Gwen::Controls::WindowCanvas::GetSizable(void)
{
	return m_Sizer->Visible();
}

bool Gwen::Controls::WindowCanvas::CanMaximize(void)
{
	return m_bCanMaximize;
}

void Gwen::Controls::WindowCanvas::SetCanMaximize( bool b )
{
	if ( m_bCanMaximize == b )
		return;

	m_bCanMaximize = b;
}

Gwen::Controls::CustomWindowCanvas::CustomWindowCanvas(int x, int y, int w, int h, Gwen::Skin::Base* pSkin, const Gwen::String & strWindowTitle, unsigned int flags)
	: BaseClass(NULL)
{
	
	m_bCanMaximize = true;
	m_bIsMaximized = false;
	SetPadding(Padding(1, 0, 1, 1));

	flags != SDL_WINDOW_BORDERLESS;

	// Centering the window on the desktop
	{
		int dw, dh;
		Gwen::Platform::GetDesktopSize(dw, dh);

		if (x < 0)
			x = (dw - w) * 0.5;
		if (y < 0)
			y = (dh - h) * 0.5;
	}
	m_pOSWindow = Gwen::Platform::CreatePlatformWindow(x, y, w, h, strWindowTitle, flags);
	m_WindowPos = Gwen::Point(x, y);
	pSkin->GetRender()->InitializeContext(this);
	pSkin->GetRender()->Init();
	m_Skin = m_pSkinChange = pSkin;
	SetSize(w, h);

	m_TitleBar = new Gwen::ControlsInternal::Dragger(this);
	m_TitleBar->SetHeight(24);
	m_TitleBar->SetPadding(Padding(0, 0, 0, 0));
	m_TitleBar->SetMargin(Margin(0, 0, 0, 0));
	m_TitleBar->Dock(Pos::Top);
	m_TitleBar->SetDoMove(false);
	m_TitleBar->onDragged.Add(this, &ThisClass::Dragger_Moved);
	m_TitleBar->onDragStart.Add(this, &ThisClass::Dragger_Start);
	m_TitleBar->onDoubleClickLeft.Add(this, &ThisClass::OnTitleDoubleClicked);
	m_Title = new Gwen::Controls::Label(m_TitleBar);
	m_Title->SetAlignment(Pos::Left | Pos::CenterV);
	m_Title->SetText(strWindowTitle);
	m_Title->Dock(Pos::Fill);
	m_Title->SetPadding(Padding(8, 0, 0, 0));
	m_Title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);
	// CLOSE
	{
		m_pClose = new Gwen::Controls::WindowCloseButton(m_TitleBar, "Close");
		m_pClose->Dock(Pos::Right);
		m_pClose->SetMargin(Margin(0, 0, 4, 0));
		m_pClose->onPress.Add(this, &CustomWindowCanvas::CloseButtonPressed);
		m_pClose->SetTabable(false);
		m_pClose->SetWindow(this);
	}
	// MAXIMIZE
	{
		m_pMaximize = new Gwen::Controls::WindowMaximizeButton(m_TitleBar, "Maximize");
		m_pMaximize->Dock(Pos::Right);
		m_pMaximize->onPress.Add(this, &CustomWindowCanvas::MaximizeButtonPressed);
		m_pMaximize->SetTabable(false);
		m_pMaximize->SetWindow(this);
	}
	// MINIMiZE
	{
		m_pMinimize = new Gwen::Controls::WindowMinimizeButton(m_TitleBar, "Minimize");
		m_pMinimize->Dock(Pos::Right);
		m_pMinimize->onPress.Add(this, &CustomWindowCanvas::MinimizeButtonPressed);
		m_pMinimize->SetTabable(false);
		m_pMinimize->SetWindow(this);
	}
	// Bottom Right Corner Sizer
	{
		m_Sizer = new Gwen::ControlsInternal::Dragger(this);
		m_Sizer->SetSize(16, 16);
		m_Sizer->SetDoMove(false);
		m_Sizer->onDragged.Add(this, &CustomWindowCanvas::Sizer_Moved);
		m_Sizer->onDragStart.Add(this, &CustomWindowCanvas::Dragger_Start);
		m_Sizer->SetCursor(Gwen::CursorType::SizeNWSE);
	}
}

void Gwen::Controls::CustomWindowCanvas::Dragger_Start(void)
{
	Gwen::Platform::GetCursorPos(m_HoldPos);
	m_HoldPos.x -= m_WindowPos.x;
	m_HoldPos.y -= m_WindowPos.y;
}

void Gwen::Controls::CustomWindowCanvas::Dragger_Moved(void)
{
	Gwen::Point p;
	Gwen::Platform::GetCursorPos(p);

	//
	// Dragged out of maximized
	//
	if (m_bIsMaximized)
	{
		float fOldWidth = Width();
		SetMaximize(false);
		// Change the hold pos to be the same distance across the titlebar of the resized window
		m_HoldPos.x = ((float)m_HoldPos.x) * ((float)Width() / fOldWidth);
		m_HoldPos.y = 10;
	}

	SetPos(p.x - m_HoldPos.x, p.y - m_HoldPos.y);
}

void Gwen::Controls::CustomWindowCanvas::Sizer_Moved(void)
{
	Gwen::Point p;
	Gwen::Platform::GetCursorPos(p);
	int w = (p.x) - m_WindowPos.x;
	int h = (p.y) - m_WindowPos.y;
	w = Clamp(w, 100, 9999);
	h = Clamp(h, 100, 9999);
	Gwen::Platform::SetBoundsPlatformWindow(m_pOSWindow, m_WindowPos.x, m_WindowPos.y, w, h);
	GetSkin()->GetRender()->ResizedContext(this, w, h);
	this->SetSize(w, h);
	BaseClass::DoThink();
	RenderCanvas();
}

void Gwen::Controls::CustomWindowCanvas::OnTitleDoubleClicked(void)
{
	if (!CanMaximize())
		return;

	SetMaximize(!m_bIsMaximized);
}

Gwen::Controls::CustomWindowCanvas::~CustomWindowCanvas(void)
{
	DestroyWindow();
}

void Gwen::Controls::CustomWindowCanvas::DoThink(void)
{
	Platform::MessagePump(m_pOSWindow, this);
	BaseClass::DoThink();
	RenderCanvas();
}

void * Gwen::Controls::CustomWindowCanvas::GetWindow(void)
{
	return m_pOSWindow;
}

Skin::Base * Gwen::Controls::CustomWindowCanvas::GetSkin(void)
{
	if (m_pSkinChange)
	{
		SetSkin(m_pSkinChange);
		m_pSkinChange = NULL;
	}

	return BaseClass::GetSkin();
}

void Gwen::Controls::CustomWindowCanvas::Render(Skin::Base * skin)
{
	bool bHasFocus = IsOnTop();

	if (bHasFocus)
		m_Title->SetTextColor(GetSkin()->Colors.Window.TitleActive);
	else
		m_Title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);

	skin->DrawWindow(this, m_TitleBar->Bottom(), bHasFocus);
}

void Gwen::Controls::CustomWindowCanvas::SetPos(int x, int y)
{
	int w, h;
	Gwen::Platform::GetDesktopSize(w, h);
	y = Gwen::Clamp(y, 0, h);
	m_WindowPos.x = x;
	m_WindowPos.y = y;
	Gwen::Platform::SetBoundsPlatformWindow(m_pOSWindow, x, y, Width(), Height());
}

bool Gwen::Controls::CustomWindowCanvas::IsOnTop(void)
{
	return Gwen::Platform::HasFocusPlatformWindow(m_pOSWindow);
}

void Gwen::Controls::CustomWindowCanvas::Layout(Skin::Base * skin)
{
	m_Sizer->BringToFront();
	m_Sizer->Position(Pos::Right | Pos::Bottom);
	BaseClass::Layout(skin);
}

bool Gwen::Controls::CustomWindowCanvas::CanMaximize(void)
{
	return m_bCanMaximize;
}

void Gwen::Controls::CustomWindowCanvas::SetCanMaximize(bool b)
{
	if (m_bCanMaximize == b)
		return;

	m_bCanMaximize = b;
	m_pMaximize->SetDisabled(!b);
}

void Gwen::Controls::CustomWindowCanvas::SetMaximize(bool b)
{
	m_bIsMaximized = b;
	m_pMaximize->SetMaximized(m_bIsMaximized);
	Gwen::Point pSize, pPos;
	Gwen::Platform::SetWindowMaximized(m_pOSWindow, m_bIsMaximized, pPos, pSize);
	SetSize(pSize.x, pSize.y);
	m_WindowPos = pPos;
	GetSkin()->GetRender()->ResizedContext(this, pSize.x, pSize.y);
	BaseClass::DoThink();
	RenderCanvas();
}

void Gwen::Controls::CustomWindowCanvas::SetSizable(bool b)
{
	m_Sizer->SetHidden(!b);
}

bool Gwen::Controls::CustomWindowCanvas::GetSizable(void)
{
	return m_Sizer->Visible();
}

void Gwen::Controls::CustomWindowCanvas::RenderCanvas(void)
{
	// If there isn't anything going on we sleep the thread for a few ms
	// This gives some cpu time back to the os. If you're using a rendering
	// method that needs continual updates, just call canvas->redraw every frame.

	if (!NeedsRedraw())
	{
		Platform::Sleep(10);
		return;
	}

	m_bNeedsRedraw = false;
	Gwen::Renderer::BaseRender* render = m_Skin->GetRender();

	if (render->BeginContext(this))
	{
		render->Begin();
		RecurseLayout(m_Skin);
		render->SetClipRegion(GetRenderBounds());
		render->SetRenderOffset(Gwen::Point(X() * -1, Y() * -1));
		render->SetScale(Scale());

		if (m_bDrawBackground)
		{
			render->SetDrawColor(m_BackgroundColor);
			render->DrawFilledRect(GetRenderBounds());
		}

		DoRender(m_Skin);
		DragAndDrop::RenderOverlay(this, m_Skin);
		ToolTip::RenderToolTip(m_Skin);
		render->End();
	}

	render->EndContext(this);
	render->PresentContext(this);
}

void Gwen::Controls::CustomWindowCanvas::DestroyWindow(void)
{
	if (m_pOSWindow)
	{
		GetSkin()->GetRender()->ShutdownContext(this);
		Gwen::Platform::DestroyPlatformWindow(m_pOSWindow);
		m_pOSWindow = NULL;
	}
}

void Gwen::Controls::CustomWindowCanvas::CloseButtonPressed(void)
{
	InputQuit();
}

void Gwen::Controls::CustomWindowCanvas::MaximizeButtonPressed(void)
{
	if (!CanMaximize())
		return;

	SetMaximize(!m_bIsMaximized);
}

void Gwen::Controls::CustomWindowCanvas::MinimizeButtonPressed(void)
{
	Gwen::Platform::SetWindowMinimized(m_pOSWindow, true);
}
