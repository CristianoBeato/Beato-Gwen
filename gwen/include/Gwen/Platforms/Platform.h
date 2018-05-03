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
		GWEN_EXPORT void Sleep( unsigned int iMS );

		/*
		===========================================================================
		Set the system cursor to iCursor
		Cursors are defined in Structures.h
		===========================================================================
		*/
		GWEN_EXPORT void SetCursor( unsigned char iCursor );

		/*
		===========================================================================
		===========================================================================
		*/
		GWEN_EXPORT void GetCursorPos( Gwen::Point & p );
		GWEN_EXPORT void GetDesktopSize( int & w, int & h );

		/*
		===========================================================================
		Used by copy/paste
		===========================================================================
		*/
		GWEN_EXPORT UnicodeString GetClipboardText();
		GWEN_EXPORT bool SetClipboardText( const UnicodeString & str );

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
		GWEN_EXPORT bool FileOpen( const String & Name, const String & StartPath, const String & Extension, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback );
		GWEN_EXPORT bool FileSave( const String & Name, const String & StartPath, const String & Extension, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback );
		GWEN_EXPORT bool FolderOpen( const String & Name, const String & StartPath, Gwen::Event::Handler* pHandler, Event::Handler::FunctionWithInformation fnCallback );

		/*
		===========================================================================
		Window Creation
		===========================================================================
		*/
		GWEN_EXPORT void* CreatePlatformWindow( int x, int y, int w, int h, const Gwen::String & strWindowTitle, unsigned int flags );
		GWEN_EXPORT void DestroyPlatformWindow( void* pPtr );
		GWEN_EXPORT void SetBoundsPlatformWindow( void* pPtr, int x, int y, int w, int h );
		GWEN_EXPORT void MessagePump( void* pWindow, Gwen::Controls::Canvas* ptarget );
		GWEN_EXPORT void EventPump(void* pWindow, Gwen::Controls::Canvas* ptarget, SDL_Event eventHandler);
		GWEN_EXPORT bool HasFocusPlatformWindow( void* pPtr );
		GWEN_EXPORT bool IsWindowVisible(void* pWindow);
		GWEN_EXPORT void SetWindowMaximized( void* pPtr, bool bMaximized, Gwen::Point & pNewPos, Gwen::Point & pNewSize );
		GWEN_EXPORT void SetWindowMinimized( void* pPtr, bool bMinimized );

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

	}
}
#endif
