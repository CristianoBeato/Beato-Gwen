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

#ifndef STB_FONT_H
#define STB_FONT_H

#include "Gwen/Fonts/Font.h"

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace Gwen
{
	namespace Font
	{
		typedef struct stbtt_packedchar stbtt_packedchar;

		class STBFont : public FontBase
		{
		public:
			STBFont(void);

			virtual void	Load(Gwen::Renderer::BaseRender* render);
			virtual void	GetCharRenderRect(int char_index, float &xpos, float &ypos, CharRect & rect);
			virtual float	GetTextLength(const Gwen::UnicodeString & text);

		private:
			stbtt_bakedchar m_cdata[96]; // ASCII 32..126 is 95 glyphs
		};
	}
}
#endif //!STB_FONT_H
