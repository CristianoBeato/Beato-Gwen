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

#ifndef GWEN_STRUCTURES_H
#define GWEN_STRUCTURES_H

#ifdef _MSC_VER
#pragma warning( disable : 4244 )
#pragma warning( disable : 4251 )
#endif

#include "Gwen/Exports.h"
#include <SDL_types.h>
#include <SDL_events.h>
#include <string>

namespace Gwen
{
	namespace Controls
	{
		class Base;
		class Canvas;
	}

	namespace CursorType
	{
		static const unsigned char Normal	= 0;
		static const unsigned char Beam		= 1;
		static const unsigned char SizeNS	= 2;
		static const unsigned char SizeWE	= 3;
		static const unsigned char SizeNWSE	= 4;
		static const unsigned char SizeNESW	= 5;
		static const unsigned char SizeAll	= 6;
		static const unsigned char No		= 7;
		static const unsigned char Wait		= 8;
		static const unsigned char Finger	= 9;

		static const unsigned char Count	= 10;
	}

#if 1
	typedef wchar_t UnicodeChar; // Portability??
	typedef std::wstring UnicodeString;
#else
	typedef Uint16 UnicodeChar;
	typedef std::basic_string<UnicodeChar, std::char_traits<UnicodeChar>, std::allocator<UnicodeChar>> UnicodeString;
#endif
	
	typedef std::string String;


	struct GWEN_EXPORT Margin
	{
		Margin( int left_ = 0, int top_ = 0, int right_ = 0, int bottom_ = 0 )
		{
			this->top = top_;
			this->bottom = bottom_;
			this->left = left_;
			this->right = right_;
		}

		Margin operator + ( const Margin & margin ) const
		{
			Margin m;
			m.top		= top + margin.top;
			m.bottom	= bottom + margin.bottom;
			m.left		= left + margin.left;
			m.right		= right + margin.right;
			return m;
		}

		int top, bottom, left, right;
	};


	typedef Margin Padding;


	struct GWEN_EXPORT Rect
	{
		Rect( int x_ = 0, int y_ = 0, int w_ = 0, int h_ = 0 )
		{
			this->x = x_;
			this->y = y_;
			this->w = w_;
			this->h = h_;
		}

		Rect operator + ( const Rect & rct ) const
		{
			Rect m;
			m.x		= x + rct.x;
			m.y		= y + rct.y;
			m.w		= w + rct.w;
			m.h		= h + rct.h;
			return m;
		}

		int x, y, w, h;
	};


	struct GWEN_EXPORT Point
	{
		Point( int x_ = 0, int y_ = 0 )
		{
			this->x = x_;
			this->y = y_;
		}

		void operator += ( const Point & p )
		{
			x += p.x;
			y += p.y;
		}

		Point operator + ( const Point & p ) const
		{
			return Point( x + p.x, p.y + y );
		}

		void operator -= ( const Point & p )
		{
			x -= p.x;
			y -= p.y;
		}

		Point operator - ( const Point & p ) const
		{
			return Point( x - p.x, y - p.y );
		}

		int x, y;
	};

	struct GWEN_EXPORT HSV
	{
		float h;
		float s;
		float v;
	};


	struct GWEN_EXPORT Color
	{
		Color( unsigned char r_ = 255, unsigned char g_ = 255, unsigned char b_ = 255, unsigned char a_ = 255 )
		{
			this->r = r_;
			this->g = g_;
			this->b = b_;
			this->a = a_;
		}

		void operator = ( Color c )
		{
			this->r = c.r;
			this->g = c.g;
			this->b = c.b;
			this->a = c.a;
		}

		void operator += ( Color c )
		{
			this->r += c.r;
			this->g += c.g;
			this->b += c.b;
			this->a += c.a;
		}

		void operator -= ( Color c )
		{
			this->r -= c.r;
			this->g -= c.g;
			this->b -= c.b;
			this->a -= c.a;
		}

		void operator *= ( float f )
		{
			this->r *= f;
			this->g *= f;
			this->b *= f;
			this->a *= f;
		}

		Color operator * ( float f )
		{
			return Color(
					   ( float ) this->r * f,
					   ( float ) this->g * f,
					   ( float ) this->b * f,
					   ( float ) this->a * f
				   );
		}

		Color operator - ( Color c )
		{
			return Color(
					   this->r - c.r,
					   this->g - c.g,
					   this->b - c.b,
					   this->a - c.a
				   );
		}

		Color operator + ( Color c )
		{
			return Color(
					   this->r + c.r,
					   this->g + c.g,
					   this->b + c.b,
					   this->a + c.a
				   );
		}

		bool operator == ( const Color & c ) const
		{
			return c.r == r && c.g == g && c.b == b && c.a == a;
		}


		unsigned char r, g, b, a;
	};


	namespace DragAndDrop
	{
		struct GWEN_EXPORT Package
		{
			Package()
			{
				userdata = NULL;
				draggable = false;
				drawcontrol = NULL;
				holdoffset = Gwen::Point( 0, 0 );
			}

			String	name;
			void*	userdata;
			bool	draggable;

			Gwen::Controls::Base*	drawcontrol;
			Gwen::Point	holdoffset;
		};
	}
}
#endif
