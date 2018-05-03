/*
	GWEN
	Copyright (c) 2012 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_WINDOWCANVAS_H
#define GWEN_CONTROLS_WINDOWCANVAS_H

#include "Gwen/Controls/Base.h"
#include "Gwen/InputHandler.h"
#include "Gwen/WindowProvider.h"
#include "Gwen/Controls/Dragger.h"
#include "Gwen/Controls/Label.h"
#include "Gwen/Controls/WindowButtons.h"

namespace Gwen
{
	namespace Controls
	{
		class GWEN_EXPORT WindowCanvas : public Canvas, public Gwen::WindowProvider
		{
			public:

				GWEN_CLASS( WindowCanvas, Controls::Canvas );

				WindowCanvas( int x, int y, int w, int h,
					Gwen::Skin::Base* pRenderer, 
					const Gwen::String & strWindowTitle = "", 
					unsigned int flags = 0 );

				~WindowCanvas(void);

				virtual void DoThink(void);

				// Gwen::WindowProvider
				virtual void* GetWindow(void);
				virtual bool InputQuit(void);

				Skin::Base* GetSkin(void);

				virtual void Render( Skin::Base* skin );

				virtual void SetPos( int x, int y );
				virtual bool IsOnTop();

				virtual void Layout( Skin::Base* skin );

				virtual bool CanMaximize(void);
				virtual void SetCanMaximize( bool b );
				virtual void SetMaximize( bool b );

				virtual void SetSizable(bool b);
				virtual bool GetSizable(void);

			protected:

				virtual void RenderCanvas(void);
				virtual void DestroyWindow(void);
				virtual void Dragger_Start(void);
				virtual void Dragger_Moved(void);
				virtual void Sizer_Moved(void);
				virtual void OnTitleDoubleClicked(void);

				Gwen::Skin::Base*			m_pSkinChange;
				Gwen::Point					m_WindowPos;
				Gwen::Point					m_HoldPos;

				void*						m_pOSWindow;
				bool						m_bCanMaximize;
				bool						m_bIsMaximized;

				ControlsInternal::Dragger*	m_Sizer;
		};

		class GWEN_EXPORT CustomWindowCanvas : public Canvas, public Gwen::WindowProvider
		{
		public:

			GWEN_CLASS(CustomWindowCanvas, Controls::Canvas);

			CustomWindowCanvas(int x, int y, int w, int h, Gwen::Skin::Base* pRenderer, const Gwen::String & strWindowTitle = "", unsigned int flags = 0);
			~CustomWindowCanvas(void);

			virtual void DoThink(void);
			
			// Gwen::WindowProvider
			virtual void* GetWindow(void);
			

			Skin::Base* GetSkin(void);

			virtual void Render(Skin::Base* skin);
			virtual void SetPos(int x, int y);
			virtual bool IsOnTop(void);

			virtual void Layout(Skin::Base* skin);

			virtual bool CanMaximize(void);
			virtual void SetCanMaximize(bool b);
			virtual void SetMaximize(bool b);

			virtual void SetSizable(bool b);
			virtual bool GetSizable(void);

		protected:
			virtual void RenderCanvas(void);
			virtual void DestroyWindow(void);

			virtual void CloseButtonPressed(void);
			virtual void MaximizeButtonPressed(void);
			virtual void MinimizeButtonPressed(void);

			virtual void Dragger_Start(void);
			virtual void Dragger_Moved(void);
			virtual void Sizer_Moved(void);
			virtual void OnTitleDoubleClicked(void);

			void*		m_pOSWindow;

			Gwen::Skin::Base*						m_pSkinChange;

			ControlsInternal::Dragger*				m_TitleBar;
			ControlsInternal::Dragger*				m_Sizer;
			Gwen::Controls::Label*					m_Title;

			Gwen::Point								m_WindowPos;
			Gwen::Point								m_HoldPos;
			bool									m_bCanMaximize;
			bool									m_bIsMaximized;
			Gwen::Controls::WindowCloseButton*		m_pClose;
			Gwen::Controls::WindowMaximizeButton*	m_pMaximize;
			Gwen::Controls::WindowMinimizeButton*	m_pMinimize;
		};
	}
}
#endif
