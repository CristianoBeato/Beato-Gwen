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

#ifndef GWEN_PLATFORM_H
#define GWEN_PLATFORM_H

#include "Gwen/Structures.h"
#include "Gwen/Events.h"

namespace Gwen
{
	namespace Platform
	{
		/*
		===========================================================================
		Force a thread, os system wait for n milliseconds
		===========================================================================
		*/
		GWEN_EXPORT void Sleep(unsigned int iMS);

		/*
		===========================================================================
		Set the system cursor to iCursor
		Cursors are defined in Structures.h
		===========================================================================
		*/
		GWEN_EXPORT void SetCursor(unsigned char iCursor);

		/*
		===========================================================================
		===========================================================================
		*/
		GWEN_EXPORT void GetCursorPos(Gwen::Point & p);
		GWEN_EXPORT void GetDesktopSize(int & w, int & h);

		/*
		===========================================================================
		Used by copy/paste
		===========================================================================
		*/
		GWEN_EXPORT UnicodeString GetClipboardText();
		GWEN_EXPORT bool SetClipboardText(const UnicodeString & str);

		/*
		===========================================================================
		Needed for things like double click
		===========================================================================
		*/
		GWEN_EXPORT float GetTimeInSeconds();

		/*
		===========================================================================
		 System Dialogs ( Can return false if unhandled )
		========================================================================== =
		*/
		GWEN_EXPORT bool FileOpen(const String & Name, const String & StartPath, const String & Extension, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback);
		GWEN_EXPORT bool FileSave(const String & Name, const String & StartPath, const String & Extension, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback);
		GWEN_EXPORT bool FolderOpen(const String & Name, const String & StartPath, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback);

		/*
		===========================================================================
		Window Creation
		===========================================================================
		*/
		GWEN_EXPORT void* CreatePlatformWindow(int x, int y, int w, int h, const Gwen::String & strWindowTitle, unsigned int flags);
		GWEN_EXPORT void DestroyPlatformWindow(void* pPtr);
		GWEN_EXPORT void SetBoundsPlatformWindow(void* pPtr, int x, int y, int w, int h);
		GWEN_EXPORT void MessagePump(void* pWindow, Gwen::Controls::Canvas* ptarget);
		GWEN_EXPORT void EventPump(void* pWindow, Gwen::Controls::Canvas* ptarget, SDL_Event eventHandler);
		GWEN_EXPORT bool HasFocusPlatformWindow(void* pPtr);
		GWEN_EXPORT bool IsWindowVisible(void* pWindow);
		GWEN_EXPORT void SetWindowMaximized(void* pPtr, bool bMaximized, Gwen::Point & pNewPos, Gwen::Point & pNewSize);
		GWEN_EXPORT void SetWindowMinimized(void* pPtr, bool bMinimized);

		//Beato Begin: File Stream Parser
		class GWEN_EXPORT Stream
		{
		public:
			Stream(void);
			virtual			~Stream(void);
			virtual bool	open(const char *name, const char *mode) = 0;
			virtual void	close(void) {}
			virtual bool	end(void) = 0;
			virtual long	tell(void) { return -1; }
			virtual bool	seek(long offset, int whence = SEEK_SET) { return false; }
			virtual long	size(void);
			virtual int		read(void *buf, int len) { return 0; }
			virtual int		write(const void *buf, int len) { return 0; }
			virtual int		getchar(void) { Uint8 c; return read(&c, 1) == 1 ? c : -1; }
			virtual bool	putchar(int n) { Uint8 c = n; return write(&c, 1) == 1; }
			virtual bool	getline(char *str, int len);
			virtual bool	putstring(const char *str) { int len = strlen(str); return write(str, len) == len; }
		private:
		};

		class GWEN_EXPORT FileStream : public Stream
		{
		public:
			FileStream(void);
			virtual ~FileStream(void);

			virtual bool	open(const char *name, const char *mode);
			virtual void	close(void);
			virtual long	tell(void);
			virtual bool	seek(long offset, int whence);
			virtual long	size(void);
			virtual bool	end(void);
			virtual int		read(void *buf, int len);
			virtual int		write(const void *buf, int len);
			virtual bool	getline(char *str, int len);
			virtual bool	putstring(const char *str);
			virtual int		printf(const char *fmt, ...);
		private:
			void*	m_fileHandler;
		};

//BEATO: TODO
#if 0
		namespace Allocators
		{
			static void	*	Alloc(size_t size);
			static void	*	Calloc(unsigned int num, size_t size);
			static void	*	Ralloc(void * ptr, size_t size);
			static void		Free(void *ptr);
			static void		Cfree(void *ptr);

			//alloc memory to a new type
			template<typename _type_>
			inline _type_ * New(void)
			{
				_type_ *ptr = new (Alloc(sizeof(_type_))) _type_();
				if (!ptr) abort();
				return ptr;
			}

			//alloc array to a new type;s
			template<typename _type_>
			inline _type_ * New(unsigned int num)
			{
				_type_ *ptr = static_cast<_type_*>(Calloc(num, sizeof(_type_)));
				if (!ptr) abort();
				return ptr;
			}

			//re alloc a exiting point
			template<typename _type_>
			inline _type_ * New(const _type_ ref)
			{
				_type_ *ptr = static_cast<_type_*>(Alloc(sizeof(_type_)));
				if (!ptr) abort();

				//copy 
				*ptr = ref;
				return ptr;
			}

			//realloc memory to a new type pointer
			template<typename _type_>
			inline _type_ * New(_type_* ptr)
			{
				_type_ *p = static_cast<_type_*>(Ralloc(ptr, sizeof(_type_)));
				if (!p) abort();
				return p;
			}

			//free ptr
			template<typename _type_>
			inline void Delete(_type_ * ptr)
			{
				reinterpret_cast<_type_ *>(ptr)->~_type_();
				Free(ptr);
			}

			//Stack dealocator
			template<typename _type_>
			inline void Delete(unsigned int num, _type_ * ptr)
			{
				SDL_stack_free(ptr)
			}
		};
#endif

		template<typename _type_>
		class GWEN_EXPORT SmartPtr
		{
		public:
			//create a empty pointer
			SmartPtr(void) : m_pointer(nullptr), m_size(0) {};
			//create the pointer from a reference
			SmartPtr(_type_ * ptr) : m_pointer(ptr) { sizeof(ptr); };
			//create the pointer whit the reference whit size
			SmartPtr(_type_ * ptr, size_t size) : m_pointer(ptr), m_size(size) {};
			//create the pointer from a reference pointer
			SmartPtr(void * ptr, size_t size) : m_pointer(ptr), m_size(size) {};
			//create a initializer
			//SmartPtr(const _type_ init){ m_pointer = Allocators::New<_type_>(init); }

			~SmartPtr(void) { clear(); }

			void clear(void)
			{
				//if not clear 
				if (m_pointer != nullptr)
				{
					//Allocators::Delete<_type_>(m_pointer);
					delete m_pointer;
					m_pointer = nullptr;
					m_size = 0;
				}
			}

			inline const _type_ * getPtr(void) const { return m_pointer; }

			inline _type_ * &getPtr(void) { return m_pointer; }

			inline const size_t getSize(void) const { return m_size; }

			inline _type_ &operator*(void) { return *m_pointer; }

			inline _type_ *operator->(void) { return m_pointer; }
		private:
			_type_	*	m_pointer;
			size_t		m_size;
		};
		//Beato End
	}
}
#endif
