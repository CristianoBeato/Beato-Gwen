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

#ifndef GWEN_FONT_H
#define GWEN_FONT_H

#include <string>
#include <list>

#include "Gwen/Gwen.h"

namespace Gwen
{
	namespace Font
	{
		typedef struct
		{
			//This is in Gwen main form, from top left
			//OpenGL Draw from bottom left to top right
			float x, y, s0, t0; // top-left
			float w, h, s1, t1; // bottom-right
		}CharRect;

		class GWEN_EXPORT FontBase
		{
		public:
			FontBase(void);
			virtual ~FontBase(void);
			virtual void	Load(Gwen::Renderer::BaseRender* render) = 0;
			virtual void	GetCharRenderRect(int char_index, float &xpos, float &ypos, CharRect & rect) = 0;
			virtual float	GetTextLength(const UnicodeString & text) = 0;

			virtual void				SetFaceName(const UnicodeString name);
			virtual void				SetSize(const float size );
			virtual void				SetBold(const bool bold);

			virtual UnicodeString &		FaceName(void);
			virtual const UnicodeString FaceName(void) const;

			virtual void*	&			GetData(void);
			virtual const float			Size(void) const;
			virtual float	&			Size(void);

			virtual Uint16				GetAtlasWidth(void) const;
			virtual Uint16				GetAtlasHeight(void) const;
			virtual Uint8				GetAtlasColorDept(void) const;
			virtual byte*				GetAtlasBuff(void) const;
			
			template<typename U> inline U* castData(void) const;
		protected:
			float				m_size;
			UnicodeString		m_facename;

			//Font gliph Atlas
			Uint16				m_AtlasWidth;
			Uint16				m_AtlasHeight;
			Uint8				m_AtlasDepth;
			byte*				m_AtlasBuff;

		private:
			bool				m_bold;

			// This should be set by the renderer
			// if it tries to use a font where it's
			// NULL.
			void*					m_data;

			// This is the real font size, after it's
			// been scaled by Render->Scale()
			float					m_realsize;
		};

		template<typename U>
		inline U * FontBase::castData(void) const
		{
			return static_cast<U*>(m_data);
		}
	}
} //namespace Gwen
#endif
