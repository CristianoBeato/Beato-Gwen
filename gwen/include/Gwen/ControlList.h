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

#ifndef GWEN_CONTROLLIST_H
#define GWEN_CONTROLLIST_H

namespace Gwen
{
	struct Point;
	class TextObject;

	namespace Controls
	{
		class Base;
	}

	namespace Event
	{
		class Handler;
		struct Information;
		struct Packet;

		typedef const Gwen::Event::Information & Info;
	}

	template < typename TYPE >
	class TEasyList
	{
		public:

			typedef std::list<TYPE> List;

			void Add( TYPE pControl )
			{
				if ( Contains( pControl ) ) { return; }

				list.push_back( pControl );
			}

			void Remove( TYPE pControl )
			{
				list.remove( pControl );
			}

			void Add( const List & list )
			{
				for ( typename List::const_iterator it = list.begin(); it != list.end(); ++it )
				{
					Add( *it );
				}
			}

			void Add( const TEasyList<TYPE> & list )
			{
				Add( list.list );
			}

			bool Contains( TYPE pControl ) const
			{
				typename List::const_iterator it = std::find( list.begin(), list.end(), pControl );
				return it != list.end();
			}

			inline void Clear()
			{
				list.clear();
			}

			List list;
	};

	class ControlList : public TEasyList<Gwen::Controls::Base*>
	{
		public:

			void Enable();
			void Disable();

			void Show();
			void Hide();

			Gwen::TextObject GetValue();
			void SetValue( const Gwen::TextObject & value );

			template <typename T>
			void SetAction( Gwen::Event::Handler* ob,
							void ( T::*f )( Gwen::Event::Info ),
							const Gwen::Event::Packet & packet )
			{
				SetActionInternal( ob,
								   static_cast<void ( Gwen::Event::Handler::* )( Gwen::Event::Info ) > ( f ),
								   packet );
			}

			void MoveBy( const Gwen::Point & point );

			void DoAction();

		protected:

			void SetActionInternal( Gwen::Event::Handler* pObject,
									void ( Gwen::Event::Handler::*f )( Gwen::Event::Info ),
									const Gwen::Event::Packet & packet );
	};

};

#endif
