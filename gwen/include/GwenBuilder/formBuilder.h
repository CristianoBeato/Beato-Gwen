/*
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
*/

#ifndef _GWEN_FORM_BUILDER_H_
#define _GWEN_FORM_BUILDER_H_

#include "Gwen/Controls.h"
#include "Gwen/Skins/TexturedBase.h"

namespace Gwen
{
	class formBuilder
	{
	public:
		formBuilder(void);
		~formBuilder(void);

		//try load the XML form file, and return the base 
		bool			formNewXml(Gwen::String formPath, int x, int y, int w, int h);
		//get the base canvas of the form
		Controls::Canvas*	getCanvas(void) const;
	private:
		TiXmlDocument	*	m_formFile;
		Controls::Canvas*	m_Canvas;
	};
}

#endif // !_GWEN_FORM_BUILDER_H_
