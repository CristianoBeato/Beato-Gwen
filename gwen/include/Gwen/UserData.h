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

#ifndef GWEN_USERDATA_H
#define GWEN_USERDATA_H

namespace Gwen
{
	/*

		Allows you to store multiple and various user data

		//
		// Valid
		//
		UserDataStorage.Set( 100.0f );
		UserDataStorage.<float>Get();

		//
		// Invalid - didn't Add a float type!
		//
		UserDataStorage.Set( 100 );
		UserDataStorage.<float>Get();

		//
		// You can pass structs as long as they can be copied safely.
		//
		UserDataStorage.Set( mystruct );
		UserDataStorage.<MyStruct>Get();

		//
		// If you pass a pointer then whatever it's pointing to
		// should be valid for the duration. And it won't be freed.
		//
		UserDataStorage.Set( &mystruct );
		UserDataStorage.<MyStruct*>Get();

	*/
	class UserDataStorage
	{
			struct ValueBase
			{
				virtual ~ValueBase() {}
				virtual void DeleteThis() = 0;
			};

			template<typename T> struct Value : public ValueBase
			{
				T val;

				Value( const T & v )
				{
					val = v;
				}

				virtual void DeleteThis()
				{
					delete this;
				}
			};

		public:

			UserDataStorage()
			{
			}

			~UserDataStorage()
			{
				std::map< Gwen::String, void*>::iterator it = m_List.begin();
				std::map< Gwen::String, void*>::iterator itEnd = m_List.end();

				while ( it != itEnd )
				{
					( ( ValueBase* ) it->second )->DeleteThis();
					++it;
				}
			}

			template<typename T>
			void Set( const Gwen::String & str, const T & var )
			{
				Value<T>* val = NULL;
				std::map< Gwen::String, void*>::iterator it = m_List.find( str );

				if ( it != m_List.end() )
				{
					( ( Value<T>* ) it->second )->val = var;
				}
				else
				{
					val = new Value<T> ( var );
					m_List[ str ] = ( void* ) val;
				}
			};

			bool Exists( const Gwen::String & str )
			{
				return m_List.find( str ) != m_List.end();
			};

			template <typename T>
			T & Get( const Gwen::String & str )
			{
				Value<T>* v = ( Value<T>* ) m_List[ str ];
				return v->val;
			}

			std::map< Gwen::String, void*>	m_List;
	};

};

#endif
