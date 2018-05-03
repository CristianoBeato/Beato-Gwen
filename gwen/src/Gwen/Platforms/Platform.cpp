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

#include "precompiled.h"
#pragma hdrstop

#include "Gwen/Platforms/Platform.h"
#include "Gwen/Gwen.h"

Gwen::Platform::Stream::Stream(void)
{
}

Gwen::Platform::Stream::~Stream(void)
{
	close();
}

long Gwen::Platform::Stream::size(void)
{
	long endpos;
	long pos = tell();

	//check if are at file end
	//if (pos < 0 || !seek(0, SEEK_END))
	if (pos < 0 || !seek(0, SEEK_END))
		return -1;

	endpos = tell();
	return pos == endpos || seek(pos, SEEK_SET) ? endpos : -1;
}

bool Gwen::Platform::Stream::getline(char * str, int len)
{
	loopi(len - 1)
	{
		if (read(&str[i], 1) != 1)
		{ 
			str[i] = '\0';
			return i > 0; 
		}
		else if (str[i] == '\n') 
		{ 
			str[i + 1] = '\0';
			return true;
		}
	}
	if (len > 0) 
		str[len - 1] = '\0';

	return true;
}

