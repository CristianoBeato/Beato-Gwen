/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_LABEL_H
#define GWEN_CONTROLS_LABEL_H

#include "Gwen/Renderers/BaseRender.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Text.h"

namespace Gwen
{
	namespace Controls
	{
		class GWEN_EXPORT Label : public Controls::Base
		{
			public:

				GWEN_CONTROL( Label, Controls::Base );
				virtual void PreDelete( Gwen::Skin::Base* skin );

				virtual void SetText( const TextObject & str, bool bDoEvents = true );

				virtual const TextObject & GetText() const { return m_Text->GetText(); }

				virtual void Render( Skin::Base* /*skin*/ ) {}

				virtual void PostLayout( Skin::Base* skin );

				virtual void SizeToContents(void);

				virtual void SetAlignment( int iAlign );
				virtual int GetAlignment(void);


				virtual void SetFont( Gwen::UnicodeString strFacename, int iSize, bool bBold );

				virtual void SetFont( Gwen::Font::FontBase* pFont ) { m_Text->SetFont( pFont ); }
				virtual Gwen::Font::FontBase* GetFont() { return m_Text->GetFont(); }
				virtual void SetTextColor( const Gwen::Color & col ) { m_Text->SetTextColor( col ); }
				virtual void SetTextColorOverride( const Gwen::Color & col ) { m_Text->SetTextColorOverride( col ); }
				inline const Gwen::Color & TextColor() const { return m_Text->TextColor(); }

				virtual int TextWidth(void) { return m_Text->Width(); }
				virtual int TextRight(void) { return m_Text->Right(); }
				virtual int TextHeight(void) { return m_Text->Height(); }
				virtual int TextX(void) { return m_Text->X(); }
				virtual int TextY(void) { return m_Text->Y(); }
				virtual int TextLength(void) { return m_Text->Length(); }

				Gwen::Rect GetCharacterPosition( int iChar );

				virtual void SetTextPadding( const Padding & padding ) { m_Text->SetPadding( padding ); Invalidate(); InvalidateParent(); }
				virtual const Padding & GetTextPadding() { return m_Text->GetPadding(); }

				inline int Alignment(void) const { return m_iAlign; }

				virtual void MakeColorNormal(void) { SetTextColor( GetSkin()->Colors.Label.Default ); }
				virtual void MakeColorBright(void) { SetTextColor( GetSkin()->Colors.Label.Bright ); }
				virtual void MakeColorDark(void) { SetTextColor( GetSkin()->Colors.Label.Dark ); }
				virtual void MakeColorHighlight() { SetTextColor( GetSkin()->Colors.Label.Highlight ); }

				virtual TextObject GetValue(void) { return GetText(); }
				virtual void SetValue( const TextObject & strValue ) { return SetText( strValue ); }

				virtual bool Wrap(void) { return m_Text->Wrap(); }
				virtual void SetWrap( bool b ) { m_Text->SetWrap( b ); }

				virtual void OnBoundsChanged( Gwen::Rect oldChildBounds );

			protected:

				virtual void OnTextChanged(void) {};

				Gwen::Font::FontBase*		m_CreatedFont;
				ControlsInternal::Text*		m_Text;
				int m_iAlign;


		};
	}
}
#endif
