/*
===========================================================================
GWEN

Copyright (c) 2010 Facepunch Studios
Copyright (c) 2017-2019 Cristiano Beato

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
#ifndef GWEN_TEXTURE_H
#define GWEN_TEXTURE_H

#include <string>

#include "Gwen/Renderers/BaseRender.h"

namespace Gwen
{
	// Texture
	class Texture
	{
	public:
		Texture(void);
		~Texture(void){}

		void	Load(const TextObject & str, Gwen::Renderer::BaseRender* render);

//Beato Begin: Internal loade
		void	Load(const TextObject & name, unsigned int width, unsigned int height, unsigned char bits,
			const char * pixelBuff, Gwen::Renderer::BaseRender * render);
//Beato End
		void	Release(Gwen::Renderer::BaseRender* render);
		bool	FailedToLoad(void) const;
		
		//void gen image from pixel src 32 bit image
		void	GenImage(Gwen::Renderer::BaseRender* render, const byte*  src, int width, int height);

		//get file size dir
		TextObject		getName(void) const;
		TextObject		&getName(void);

		//get render image data
		const Uint32	getWidth(void) const;
		Uint32			&getWidth(void);
		const Uint32	getHeight(void) const;
		Uint32			&getHeight(void);
		const Uint8		getBitPP(void) const;
		Uint8			&getBitPP(void);
		const void*		getData(void) const;
		void*			&getData(void);

		//get image in renderer format
		template<typename U> inline U* castData(void) const;

	private:
		typedef std::list<Texture*>		List;
		TextObject		name;
		void*			data;
		bool			failed;
		Uint8			bpp;	//bytes per pixel
		Uint32			width;
		Uint32			height;
	};

	template<typename U>
	inline U * Texture::castData(void) const
	{
		if (!data)
			return nullptr;

		return static_cast<U*>(data);
	}

}
#endif
