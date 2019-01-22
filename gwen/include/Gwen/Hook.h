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

#ifndef GWEN_HOOK_H
#define GWEN_HOOK_H

#include "Gwen/Gwen.h"
#include <list>

#ifdef GWEN_HOOKSYSTEM

namespace Gwen
{
	namespace Hook
	{
		class GWEN_EXPORT BaseHook
		{
			public:

				virtual bool OnControlClicked( Gwen::Controls::Base*, int /*iMouseX*/, int /*iMouseY*/ ) { return false; };
		};

		typedef std::list<BaseHook*> HookList;

		GWEN_EXPORT HookList & GetHookList();

		GWEN_EXPORT void AddHook( BaseHook* pHook );
		GWEN_EXPORT void RemoveHook( BaseHook* pHook );

		template< typename fnc >
		bool  CallHook( fnc f )
		{
			for ( HookList::iterator it = GetHookList().begin(); it != GetHookList().end(); ++it )
			{
				if ( ( ( *it )->*f )() ) { return true; }
			}

			return false;
		}

		template< typename fnc, typename AA >
		bool CallHook( fnc f, AA a )
		{
			for ( HookList::iterator it = GetHookList().begin(); it != GetHookList().end(); ++it )
			{
				if ( ( ( *it )->*f )( a ) ) { return true; }
			}

			return false;
		}

		template< typename fnc, typename AA, typename AB >
		bool CallHook( fnc f, AA a, AB b )
		{
			for ( HookList::iterator it = GetHookList().begin(); it != GetHookList().end(); ++it )
			{
				if ( ( ( *it )->*f )( a, b ) ) { return true; }
			}

			return false;
		}

		template< typename fnc, typename AA, typename AB, typename AC >
		bool CallHook( fnc f, AA a, AB b, AC c )
		{
			for ( HookList::iterator it = GetHookList().begin(); it != GetHookList().end(); ++it )
			{
				if ( ( ( *it )->*f )( a, b, c ) ) { return true; }
			}

			return false;
		}
	}

}

#endif
#endif
