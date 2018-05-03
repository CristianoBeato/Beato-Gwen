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

#pragma once
#ifndef GWEN_FONT_H
#define GWEN_FONT_H

#include <string>
#include <list>

//#include "Gwen/Gwen.h"
#include "Gwen/Renderers/BaseRender.h"

namespace Gwen
{

	struct Font
	{
		typedef std::list<Font*>	List;

		Font()
		{
			data = NULL;
			facename = L"Arial";
			size = 10;
			realsize = 0;
			bold = false;
		}

		void Load(Gwen::Renderer::Base* render)
		{
			//Gwen::Debug::AssertCheck(render != NULL, "No renderer!");
			render->LoadFont(this);
		}

		UnicodeString	facename;
		float size;
		bool  bold;

		// This should be set by the renderer
		// if it tries to use a font where it's
		// NULL.
		void*	data;

		// This is the real font size, after it's
		// been scaled by Render->Scale()
		float realsize;

	};

} //namespace Gwen
#endif
