/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
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

		void	Load(const TextObject & str, Gwen::Renderer::Base* render);

//Beato Begin: Internal loade
		void	Load(const TextObject & name, unsigned int width, unsigned int height, unsigned char bits,
			const char * pixelBuff, Gwen::Renderer::Base * render);
//Beato End
		void	Release(Gwen::Renderer::Base* render);
		bool	FailedToLoad(void) const;
		
		//void gen image from pixel src 32 bit image
		void	GenImage(Gwen::Renderer::Base* render, const byte*  src, int width, int height);

		//get file size dir
		TextObject		getName(void) const;
		TextObject		&getName(void);

		//get render image data
		int				getWidth(void) const;
		int				getHeight(void) const;
		void*			getData(void) const;
		int				&getWidth(void);
		int				&getHeight(void);
		void*			&getData(void);

		//get image in renderer format
		template<typename U> inline U* castData(void) const;

	private:
		typedef std::list<Texture*>		List;
		TextObject	name;
		void*	data;
		bool	failed;
		int		width;
		int		height;
	};

	template<typename U>
	inline U * Texture::castData(void) const
	{
		return static_cast<U*>(this->data);
	}

}
#endif
