#include <iostream>
#include <stdlib.h>
#include <SDL.h>
//#include <SDL_main.h>

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"
#include "UnitTest/UnitTest.h"
#include "Gwen/Input/SDL12.h"
#include "Gwen/Renderers/SDL.h"

SDL_Surface* screen = NULL;


#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv)
{
	// Create a new window
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);;

	// Create a GWEN OpenGL Renderer
	Gwen::Renderer::Base* pRenderer = new Gwen::Renderer::SDL(screen);

	pRenderer->Init();

	// Create a GWEN skin
	Gwen::Skin::TexturedBase* pSkin = new Gwen::Skin::TexturedBase( pRenderer );
	pSkin->Init( "DefaultSkin.png" );

	// Create a Canvas (it's root, on which all other GWEN panels are created)
	Gwen::Controls::Canvas* pCanvas = new Gwen::Controls::Canvas( pSkin );
	pCanvas->SetSize( 998, 650 - 24 );
	pCanvas->SetDrawBackground( true );
	pCanvas->SetBackgroundColor( Gwen::Color( 150, 170, 170, 255 ) );

	// Create our unittest control (which is a Window with controls in it)
	UnitTest* pUnit = new UnitTest( pCanvas );
	pUnit->SetPos( 10, 10 );

	// Create a Windows Control helper
	// (Processes Windows MSG's and fires input at GWEN)
	Gwen::Input::SDL12 GwenInput;
	GwenInput.Initialize( pCanvas );

	// Begin the main game loop
	SDL_Event msg;

	while ( true )
	{
		// Skip out if the window is closed
		//if ( !IsWindowVisible( g_pHWND ) )
			//break;

		// If we have a message from windows..
		if (SDL_PollEvent(&msg))
		{
			// .. give it to the input handler to process
			GwenInput.ProcessEvent(&msg);

			// if it's QUIT then quit..
			if (msg.type == SDL_QUIT)
				break;

		}

		// Main OpenGL Render Loop
		{
			//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			pCanvas->RenderCanvas();
			//SwapBuffers( GetDC( g_pHWND ) );
		}
	}

	// Clean up OpenGL
	delete pCanvas;
	delete pSkin;
	delete pRenderer;
	return 0;
}
