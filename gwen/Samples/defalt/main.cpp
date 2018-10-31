#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL.h>

#include "Gwen/Gwen.h"
#include "UnitTest/UnitTest.h"
#include "Gwen/Skins/SkinBase.h"
#include "Gwen/Controls.h"

Gwen::Renderer::Base*					m_ConsoleRenderer;
Gwen::Controls::Canvas*			m_ConsoleCanvas;

static const int k_consWidth = 540;
static const int k_consHeight = 474;
#define APP_NAME						"DOOM 3: BFG Edition"		// appears on window titles and errors


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
#if 1
	m_ConsoleRenderer = new Gwen::Renderer::SDL();
#else
	m_ConsoleRenderer = new Gwen::Renderer::OpenGL();
#endif

	// Create a GWEN skin
	Gwen::Skin::Base* pSkin = new Gwen::Skin::SkinBase(m_ConsoleRenderer);
	//preload the font
	// The fonts work differently in sdl - it can't use
	// system fonts. So force the skin to use a local one.

	// Create a Canvas (it's root, on which all other GWEN panels are created)
#if 0
	m_ConsoleCanvas = new Gwen::Controls::WindowCanvas(-1, -1, k_consWidth, k_consHeight, pSkin, APP_NAME, SDL_WINDOW_SHOWN);
#else
	m_ConsoleCanvas = new Gwen::Controls::CustomWindowCanvas(-1, -1, k_consWidth, k_consHeight, pSkin, APP_NAME, SDL_WINDOW_SHOWN);
#endif

	//load skin definition
	if (pSkin->Init("bin/DefaultSkin.xml") != true);
	{
		printf("can't load the skin definition");
		exit(-1);
	}
	
	m_ConsoleCanvas->SetDrawBackground(true);
	m_ConsoleCanvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

	Gwen::Controls::StatusBar *m_StatusBar = new Gwen::Controls::StatusBar(m_ConsoleCanvas);
	m_StatusBar->SetPadding(Gwen::Padding(0, 0, 0, 0));
	m_StatusBar->SetMargin(Gwen::Margin(0, 0, 0, 0));
	m_StatusBar->Dock(Gwen::Pos::Bottom);
	m_StatusBar->SendToBack();

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
