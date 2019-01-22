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

#include "Gwen/Events.h"

using namespace Gwen;
using namespace Gwen::Event;


Handler::Handler(void)
{
}

Handler::~Handler(void)
{
	CleanLinks();
}

void Handler::CleanLinks(void)
{
	// Tell all the callers that we're dead
	std::list<Caller*>::iterator iter = m_Callers.begin();

	while ( iter != m_Callers.end() )
	{
		Caller* pCaller = *iter;
		UnRegisterCaller( pCaller );
		pCaller->RemoveHandler( this );
		iter = m_Callers.begin();
	}
}

void Handler::RegisterCaller( Caller* pCaller )
{
	m_Callers.push_back( pCaller );
}

void Handler::UnRegisterCaller( Caller* pCaller )
{
	m_Callers.remove( pCaller );
}


Caller::Caller(void)
{
}

Caller::~Caller(void)
{
	CleanLinks();
}

void Caller::CleanLinks(void)
{
	std::list<handler>::iterator iter;

	for ( iter = m_Handlers.begin(); iter != m_Handlers.end(); ++iter )
	{
		handler & h = *iter;
		h.pObject->UnRegisterCaller( this );
	}

	m_Handlers.clear();
}

void Caller::Call( Controls::Base* pThis )
{
	static Gwen::Event::Information info;
	info.Control = pThis;
	Call( pThis, info );
}

void Caller::Call( Controls::Base* pThis, Gwen::Event::Info information )
{
	Gwen::Event::Information info;
	info = information;
	info.ControlCaller	= pThis;
	std::list<handler>::iterator iter;

	for ( iter = m_Handlers.begin(); iter != m_Handlers.end(); ++iter )
	{
		handler & h = *iter;
		info.Data = h.Data;

		if ( h.fnFunction )
		{ ( h.pObject->*h.fnFunction )( pThis ); }

		if ( h.fnFunctionInfo )
		{ ( h.pObject->*h.fnFunctionInfo )( info ); }

		if ( h.fnFunctionBlank )
		{ ( h.pObject->*h.fnFunctionBlank )(); }

		if ( h.fnGlobalFunction )
		{ ( *h.fnGlobalFunction )( pThis ); }

		if ( h.fnGlobalFunctionInfo )
		{ ( *h.fnGlobalFunctionInfo )( info ); }

		if ( h.fnGlobalFunctionBlank )
		{ ( *h.fnGlobalFunctionBlank )(); }
	}
}

void Caller::AddInternal( Event::Handler* pObject, Event::Handler::Function pFunction )
{
	handler h;
	h.fnFunction = pFunction;
	h.pObject = pObject;
	m_Handlers.push_back( h );
	pObject->RegisterCaller( this );
}

void Caller::AddInternal( Event::Handler* pObject, Handler::FunctionWithInformation pFunction )
{
	AddInternal( pObject, pFunction, NULL );
}

void Caller::AddInternal( Event::Handler* pObject, Handler::FunctionWithInformation pFunction, void* data )
{
	handler h;
	h.fnFunctionInfo	= pFunction;
	h.pObject			= pObject;
	h.Data				= data;
	m_Handlers.push_back( h );
	pObject->RegisterCaller( this );
}

void Caller::AddInternal( Event::Handler* pObject, Handler::FunctionBlank pFunction )
{
	handler h;
	h.fnFunctionBlank = pFunction;
	h.pObject = pObject;
	m_Handlers.push_back( h );
	pObject->RegisterCaller( this );
}

void Caller::AddInternal( Event::Handler* pObject, Event::Handler::GlobalFunction pFunction )
{
	handler h;
	h.fnGlobalFunction = pFunction;
	h.pObject = pObject;
	m_Handlers.push_back( h );
	pObject->RegisterCaller( this );
}

void Caller::AddInternal( Event::Handler* pObject, Handler::GlobalFunctionWithInformation pFunction )
{
	AddInternal( pObject, pFunction, NULL );
}

void Caller::AddInternal( Event::Handler* pObject, Handler::GlobalFunctionWithInformation pFunction, void* data )
{
	handler h;
	h.fnGlobalFunctionInfo	= pFunction;
	h.pObject				= pObject;
	h.Data					= data;
	m_Handlers.push_back( h );
	pObject->RegisterCaller( this );
}

void Caller::AddInternal( Event::Handler* pObject, Handler::GlobalFunctionBlank pFunction )
{
	handler h;
	h.fnGlobalFunctionBlank = pFunction;
	h.pObject = pObject;
	m_Handlers.push_back( h );
	pObject->RegisterCaller( this );
}

void Caller::RemoveHandler( Event::Handler* pObject )
{
	pObject->UnRegisterCaller( this );
	std::list<handler>::iterator iter = m_Handlers.begin();

	while ( iter != m_Handlers.end() )
	{
		handler & h = *iter;

		if ( h.pObject == pObject )
		{
			iter = m_Handlers.erase( iter );
		}
		else
		{
			++iter;
		}
	}
}
