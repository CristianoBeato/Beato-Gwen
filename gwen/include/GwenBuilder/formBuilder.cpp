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

#include "formBuilder.h"

#include "Gwen/Gwen.h"
#include "Gwen/Platforms/Platform.h"
//#include "contrib/tinyxml2/tinyxml.h"

Gwen::formBuilder::formBuilder(void)
{
}

Gwen::formBuilder::~formBuilder(void)
{
}

bool Gwen::formBuilder::formNewXml(Gwen::String formPath, int x, int y, int w, int h)
{
	Gwen::Platform::Stream *filesrc;
	const char * docSrc;
	//TODO: Open the source file here

	//process document
	m_formFile = new TiXmlDocument;
	m_formFile->Parse(docSrc);

	if (!m_formFile->Error())
	{
		return false;
	}

	//TODO: process if are a canvas or a window

	//TODO: process window childs

	return true;
}

Gwen::Controls::Canvas * Gwen::formBuilder::getCanvas(void) const
{
	return m_Canvas;
}
