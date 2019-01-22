#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL.h>

#include "Gwen/Gwen.h"
#include "UnitTest/UnitTest.h"
#include "Gwen/Skins/SkinBase.h"
#include "Gwen/Skins/TexturedBase.h"
#include "Gwen/Controls.h"

Gwen::Renderer::BaseRender*		m_ConsoleRenderer;
Gwen::Controls::Canvas*			m_ConsoleCanvas;

int main(int argc, char *argv[])
{
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // initialize SDL video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }
	
	// Create a GWEN Renderer
#if 0
	m_ConsoleRenderer = new Gwen::Renderer::SDL();
#else
	m_ConsoleRenderer = new Gwen::Renderer::OpenGL();
#endif

	// Create a GWEN skin
#if 0
	Gwen::Skin::SkinBase* pSkin = new Gwen::Skin::SkinBase(m_ConsoleRenderer);
#else
	Gwen::Skin::TexturedBase* pSkin = new Gwen::Skin::TexturedBase(m_ConsoleRenderer);
#endif
	//preload the font
	// The fonts work differently in sdl - it can't use
	// system fonts. So force the skin to use a local one.

	// Create a Canvas (it's root, on which all other GWEN panels are created)
#if 0
	m_ConsoleCanvas = new Gwen::Controls::WindowCanvas(-1, -1, 640, 420, pSkin, "TestWindow", SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
#else
	m_ConsoleCanvas = new Gwen::Controls::CustomWindowCanvas(-1, -1, 640, 420, pSkin, "TestWindow", SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
#endif

	//load skin definition
	//pSkin->Init("DefaultSkin.xml");
	pSkin->Init("DefaultSkin.bmp");

	pSkin->SetDefaultFont(L"OpenSans.ttf", 12);
	
	m_ConsoleCanvas->SetDrawBackground(true);
	m_ConsoleCanvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

	{
		Gwen::Controls::StatusBar *m_StatusBar = new Gwen::Controls::StatusBar(m_ConsoleCanvas);
		m_StatusBar->SetPadding(Gwen::Padding(0, 0, 0, 0));
		m_StatusBar->SetMargin(Gwen::Margin(0, 0, 0, 0));
		m_StatusBar->Dock(Gwen::Pos::Bottom);
		m_StatusBar->SendToBack();
	}
	{
		Gwen::Controls::Layout::Tile * ButtonTabble = new Gwen::Controls::Layout::Tile(m_ConsoleCanvas);
		ButtonTabble->SetPadding(Gwen::Padding(0, 0, 0, 0));
		ButtonTabble->SetMargin(Gwen::Margin(4, 2, 4, 2));
		ButtonTabble->SetHeight(24);
		ButtonTabble->Dock(Gwen::Pos::CenterH | Gwen::Pos::Bottom);
		//Copy Btn
		{
			Gwen::Controls::Button * CopyBtn = new Gwen::Controls::Button(ButtonTabble, "Copy");
			CopyBtn->SetText(L"copy");
			CopyBtn->SetWidth(72);
			//CopyBtn->onPress.Add(this, &Console::onButtonCopy);
			CopyBtn->Dock(Gwen::Pos::Left);
		}
		//Clear Btn
		{
			Gwen::Controls::Button * ClerBtn = new Gwen::Controls::Button(ButtonTabble, "Clear");
			ClerBtn->SetText(L"Clear");
			ClerBtn->SetWidth(72);
			ClerBtn->SetMargin(Gwen::Margin(2, 0, 0, 0));
			//ClerBtn->onPress.Add(this, &Console::onButtonClear);
			ClerBtn->Dock(Gwen::Pos::Left);
		}
		//Quit Btn
		{
			Gwen::Controls::Button * QutnBtn = new Gwen::Controls::Button(ButtonTabble, "Quit");
			QutnBtn->SetText(L"Quit");
			QutnBtn->SetWidth(72);
			//QutnBtn->onPress.Add(this, &Console::onButtonQuit);
			QutnBtn->Dock(Gwen::Pos::Right);
		}
	}

	// program main loop
	bool m_done = false;
	while (!m_done)
	{
		// if it's QUIT then quit..
		if (m_ConsoleCanvas->WantsQuit())
			m_done = true;

		// DRAWING STARTS HERE
		m_ConsoleCanvas->DoThink();
	} // end main loop

	delete m_ConsoleCanvas;
	delete m_ConsoleRenderer;
	//delete pSkin;

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
