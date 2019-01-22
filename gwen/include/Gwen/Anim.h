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

#ifndef GWEN_ANIM_H
#define GWEN_ANIM_H

#ifndef GWEN_NO_ANIMATION

namespace Gwen
{
	namespace Anim
	{
		class GWEN_EXPORT Animation
		{
			public:

				typedef std::list<Animation*> ChildList;
				typedef std::map< Gwen::Controls::Base*, ChildList > List;

				virtual void Think() = 0;
				virtual bool Finished() { return false; }

				virtual ~Animation() {}

				Gwen::Controls::Base*	m_Control;
		};

		GWEN_EXPORT void Add( Gwen::Controls::Base* control, Animation* animation );
		GWEN_EXPORT void Cancel( Gwen::Controls::Base* control );
		GWEN_EXPORT void Think();

		//
		// Timed animation. Provides a useful base for animations.
		//
		class GWEN_EXPORT TimedAnimation : public Animation
		{
			public:

				TimedAnimation( float fLength, float fDelay = 0.0f, float fEase = -1.0f );

				virtual void Think();
				virtual bool Finished();

				//
				// These are the magic functions you should be overriding
				//
				virtual void OnStart() {}
				virtual void Run( float /*delta*/ ) {}
				virtual void OnFinish() {}

			protected:

				bool	m_bStarted;
				bool	m_bFinished;
				float	m_fStart;
				float	m_fEnd;
				float	m_fEase;
		};

		namespace Size
		{
			class GWEN_EXPORT Height : public TimedAnimation
			{
				public:

					Height( int iStartSize, int iEndSize, float fLength, bool bHide = false, float fDelay = 0.0f, float fEase = -1.0f ) : TimedAnimation( fLength, fDelay, fEase )
					{
						m_iStartSize = iStartSize;
						m_iDelta = iEndSize - m_iStartSize;
						m_bHide = bHide;
					}

					virtual void OnStart() { m_Control->SetHeight( m_iStartSize ); }
					virtual void Run( float delta ) { m_Control->SetHeight( m_iStartSize + ( ( ( float ) m_iDelta ) * delta ) ); }
					virtual void OnFinish() { m_Control->SetHeight( m_iStartSize + m_iDelta ); m_Control->SetHidden( m_bHide ); }

				protected:

					int		m_iStartSize;
					int		m_iDelta;
					bool	m_bHide;
			};

			class Width : public TimedAnimation
			{
				public:

					Width( int iStartSize, int iEndSize, float fLength, bool bHide = false, float fDelay = 0.0f, float fEase = -1.0f ) : TimedAnimation( fLength, fDelay, fEase )
					{
						m_iStartSize = iStartSize;
						m_iDelta = iEndSize - m_iStartSize;
						m_bHide = bHide;
					}

					virtual void OnStart() { m_Control->SetWidth( m_iStartSize ); }
					virtual void Run( float delta ) { m_Control->SetWidth( m_iStartSize + ( ( ( float ) m_iDelta ) * delta ) ); }
					virtual void OnFinish() { m_Control->SetWidth( m_iStartSize + m_iDelta ); m_Control->SetHidden( m_bHide ); }

				protected:

					int		m_iStartSize;
					int		m_iDelta;
					bool	m_bHide;
			};
		}

		namespace Pos
		{
			class GWEN_EXPORT X : public Anim::TimedAnimation
			{
				public:

					X( int iStartSize, int iEndSize, float fLength, bool bHide = false, float fDelay = 0.0f, float fEase = 1.0f ) : TimedAnimation( fLength, fDelay, fEase )
					{
						m_iStartSize = iStartSize;
						m_iDelta = iEndSize - m_iStartSize;
						m_bHide = bHide;
					}

					virtual void OnStart() { m_Control->SetPos( m_iStartSize, m_Control->GetPos().y ); }
					virtual void Run( float delta ) { m_Control->SetPos( m_iStartSize + ( ( ( float ) m_iDelta ) * delta ), m_Control->GetPos().y ); }
					virtual void OnFinish() { m_Control->SetPos( m_iStartSize + m_iDelta, m_Control->GetPos().y ); m_Control->SetHidden( m_bHide ); }

				protected:

					int		m_iStartSize;
					int		m_iDelta;
					bool	m_bHide;
			};

			class Y : public Anim::TimedAnimation
			{
				public:

					Y( int iStartSize, int iEndSize, float fLength, bool bHide = false, float fDelay = 0.0f, float fEase = 1.0f ) : TimedAnimation( fLength, fDelay, fEase )
					{
						m_iStartSize = iStartSize;
						m_iDelta = iEndSize - m_iStartSize;
						m_bHide = bHide;
					}

					virtual void OnStart() { m_Control->SetPos( m_Control->GetPos().x, m_iStartSize ); }
					virtual void Run( float delta ) { m_Control->SetPos( m_Control->GetPos().x, m_iStartSize + ( ( ( float ) m_iDelta ) * delta ) ); }
					virtual void OnFinish() { m_Control->SetPos( m_Control->GetPos().x, m_iStartSize + m_iDelta ); m_Control->SetHidden( m_bHide ); }

				protected:

					int		m_iStartSize;
					int		m_iDelta;
					bool	m_bHide;
			};
		}

		namespace Tools
		{
			class Remove : public TimedAnimation
			{
				public:

					Remove( float fDelay = 0.0f ) : TimedAnimation( 0.0f, fDelay ) {}
					virtual void OnFinish() { m_Control->DelayedDelete(); }
			};
		}


	}
}

#endif
#endif
