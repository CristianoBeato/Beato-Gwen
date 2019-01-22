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

#include "Gwen/Renderers/OpenGL.h"
#include <math.h>

static const char* k_vsSource =
{
	"#version 400 core\n"
	"\n"
	"layout(location = 0) in vec2 position;\n"
	"layout(location = 1) in vec2 textCoord;\n"
	"layout(location = 2) in vec4 color;\n"
	"\n"
	"out vec2 Texcoord;\n"
	"out vec4 vertColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	Texcoord = textCoord;\n"
	"	vertColor = color;\n"
	"	gl_Position = vec4(position.xy, 0.0f, 1.0f);\n"
	"}\n"
};

static const char* k_fsSource =
{
	"#version 400 core\n"
	"\n"
	"uniform sampler2D Texture;\n"
	"uniform int blendMode;\n"
	"\n"
	"out vec4 FragColor;\n"
	"in vec2 Texcoord;"
	"in vec4 vertColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	if (blendMode == 0) //vertex color only\n"
	"	{\n"
	"		FragColor = vertColor;\n"
	"	}\n"
	"	else if (blendMode == 1) //texture color only\n"
	"	{\n"
	"		FragColor = texture(Texture, Texcoord);\n"
	"	}\n"
	"	else if (blendMode == 2)	//texture * color\n"
	"	{\n"
	"		FragColor = vec4(texture(Texture, Texcoord).rgb * vertColor.rgb, 1.0f);\n"
	"	}\n"
	"}\n"
};

enum
{
	LD_COLOR_RECT = 0,
	LD_TEXTURE_RECT,
	LD_TEXT_RECT,
	LD_LINE,
	LD_POINT
};

Gwen::Renderer::OpenGL::OpenGL(void)
{
	m_pContext = NULL;
	m_VAO = 0;
	m_VBO = 0;
	vsProgram = 0;
	fsProgram = 0;
	pipeline = 0;
	m_numVertexes = 0;
}

Gwen::Renderer::OpenGL::~OpenGL(void)
{
}

/*
====================================================
Program stages helpers
====================================================
*/
static void GetGLErrName(void)
{
//don't get gl error if not in debug mode
#ifndef _DEBUG
	return;
#endif //!_DEBUG


	const char * errType;
	GLenum ErrorCode = glGetError();
	if (ErrorCode == GL_NO_ERROR)
		return;
	else
	{
		switch (ErrorCode)
		{
		case GL_INVALID_ENUM:
			errType = "INVALID ENUM";
			break;
		case GL_INVALID_VALUE:
			errType = "INVALID VALUE";
			break;
		case GL_INVALID_OPERATION:
			errType = "INVALID OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errType = "STACK OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errType = "STACK UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errType = "GL_OUT_OF_MEMORY";
			break;
		default:
		{
			char *Err = new char;
			sprintf(Err, "Unknow GL ERROR: %i", ErrorCode);
			errType = Err;
		}
		break;
		}

		printf("GL ERROR : %s\n", errType);
	}
}

//check program for errors
static bool checkProgram(const GLuint handler)
{
	GLint result = GL_FALSE;

	//not a valid program 
	if (handler == 0)
		return false;

#if 1
	//validate the program
	glValidateProgramPipeline(handler);
	GetGLErrName();
#endif

	//check for compile errors
	glGetProgramiv(handler, GL_VALIDATE_STATUS, &result);
	if (result != GL_TRUE)
		return false;

	return true;
}

//print program log
static void	printProgramLog(const GLuint handler)
{
	int infoLogLength = 0;
	char* logBuffer;
	glGetProgramiv(handler, GL_INFO_LOG_LENGTH, &infoLogLength);
	logBuffer = (char*)SDL_calloc(infoLogLength, sizeof(char));

	//get log of the error
	glGetProgramInfoLog(handler, infoLogLength, NULL, logBuffer);
	printf(logBuffer);
	SDL_free(logBuffer);
	logBuffer = nullptr;
}

void Gwen::Renderer::OpenGL::Init(void)
{
	/*
	====================================================
	GENERATE THE VERTEX ARRAYS
	====================================================
	*/
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	GetGLErrName();
	
	glBindVertexArray(m_VAO);
	GetGLErrName();

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	glBindVertexArray(0);
	/*
	====================================================
	GENERATE THE RENDER PROGRAMS
	====================================================
	*/
	vsProgram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &k_vsSource);
#ifdef _DEBUG
	if (!checkProgram(vsProgram))
		printProgramLog(vsProgram);
#endif
	fsProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &k_fsSource);
#ifdef _DEBUG
	if (!checkProgram(fsProgram))
		printProgramLog(fsProgram);
#endif	
	glGenProgramPipelines(1, &pipeline);
	glBindProgramPipeline(pipeline);
	glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vsProgram);
	glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fsProgram);
	GetGLErrName();

	m_uTexture =	glGetUniformLocation(fsProgram, "Texture");
	m_ublendMode = glGetUniformLocation(fsProgram, "blendMode");
	GetGLErrName();

	glBindProgramPipeline(0);
}

void Gwen::Renderer::OpenGL::ShutDonw(void)
{
	glDeleteProgramPipelines(1, &pipeline);
	glDeleteProgram(fsProgram);
	glDeleteProgram(vsProgram);

	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Gwen::Renderer::OpenGL::Begin(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 1.0f);
	glEnable(GL_BLEND);
}

void Gwen::Renderer::OpenGL::End(void)
{
	Flush();
}

void Gwen::Renderer::OpenGL::Flush(void)
{
	GLenum DType = GL_TRIANGLES;
	int blendColorMode = 0;
	if (m_numVertexes == 0)
		return;
	
	if (m_lastDraw == LD_COLOR_RECT)
	{
		DType = GL_QUADS;
		blendColorMode = 0;
	}
	else if (m_lastDraw == LD_TEXTURE_RECT)
	{
		DType = GL_QUADS;
		blendColorMode = 1;
	}
	else if (m_lastDraw == LD_TEXT_RECT)
	{
		DType = GL_QUADS;
		blendColorMode = 0;
	}
	else if (m_lastDraw == LD_LINE)
		DType = GL_LINES;
	else if (m_lastDraw == LD_POINT)
		DType = GL_POINTS;

	//avoid external interferences
	glUseProgram(0);

	//start render
	glBindProgramPipeline(pipeline);
	GetGLErrName();
	
	//set the uniforms
	glProgramUniform1i(fsProgram, m_uTexture, 0); //set texture bind location
	glProgramUniform1i(fsProgram, m_ublendMode, blendColorMode); //set the color blending
	GetGLErrName();

	//bind the vertex array
	glBindVertexArray(m_VAO);
	GetGLErrName();

	//send vertexes to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * m_numVertexes, m_Vertices, GL_DYNAMIC_DRAW);
	GetGLErrName();

	//enable attribs
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, positions));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, textcoord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, vertColor));
	GetGLErrName();

	//draw the instances
	glDrawArrays(DType, 0, (GLsizei)m_numVertexes);
	GetGLErrName();

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindProgramPipeline(0);

	m_numVertexes = 0;
}

void Gwen::Renderer::OpenGL::AddVert(float x, float y, float u, float v)
{
	if (m_numVertexes >= k_maxVertex - 1)
		Flush();

	//set positions and textCoords
	m_Vertices[m_numVertexes].positions[0] = x;
	m_Vertices[m_numVertexes].positions[1] = y;

	m_Vertices[m_numVertexes].textcoord[0] = u;
	m_Vertices[m_numVertexes].textcoord[1] = v;

	m_Vertices[m_numVertexes].vertColor[0] = m_Color.r;
	m_Vertices[m_numVertexes].vertColor[1] = m_Color.g;
	m_Vertices[m_numVertexes].vertColor[2] = m_Color.b;
	m_Vertices[m_numVertexes].vertColor[3] = m_Color.a;
	
	m_numVertexes++;
}

void Gwen::Renderer::OpenGL::StartClip(void)
{
	float ScissorRect[4];
	Flush();

	Gwen::Rect rect = ClipRegion();
	float scale = Scale();

	ScissorRect[0] = rect.x;
	ScissorRect[1] = rect.y;
	ScissorRect[2] = rect.w;
	ScissorRect[3] = rect.h;

	// OpenGL's coords are from the bottom left
	// so we need to translate them here.
	ScissorRect[1] = m_Height - (ScissorRect[1] + ScissorRect[3]);
	ScissorRect[1] *= scale;
	ScissorRect[2] *= scale;
	ScissorRect[3] *= scale;
	ScissorRect[4] *= scale;

	glEnable(GL_SCISSOR_TEST);
	glScissor(ScissorRect[0], ScissorRect[1], ScissorRect[2], ScissorRect[3]);
}

void Gwen::Renderer::OpenGL::EndClip(void)
{
	Flush();
	glDisable(GL_SCISSOR_TEST);
};

void Gwen::Renderer::OpenGL::DrawFilledRect(Gwen::Rect rect)
{
	float renderRect[4];
	if (m_lastDraw != LD_COLOR_RECT)
	{
		if (m_lastDraw == LD_TEXTURE_RECT)
			UnbindTexture();

		Flush();
	}
	
	Translate(rect);

	//resize to openGl view port size
	renderRect[0] = ((float)rect.x / (float)m_Width);
	renderRect[1] = ((float)rect.y / (float)m_Height);
	renderRect[2] = ((float)(rect.x + rect.w) / (float)m_Width);
	renderRect[3] = ((float)(rect.y + rect.h) / (float)m_Height);
	renderRect[1] *= -1;
	renderRect[3] *= -1;

	AddVert(renderRect[0], renderRect[1]); //BL
	AddVert(renderRect[2], renderRect[1]); //BR
	AddVert(renderRect[2], renderRect[3]); //TR
	AddVert(renderRect[0], renderRect[3]); //TL

	m_lastDraw = LD_COLOR_RECT;
}

void Gwen::Renderer::OpenGL::DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2)
{
	float renderRect[4];
	if (!pTexture)
		return DrawMissingImage(rect);

	// Missing image, not loaded properly?
	GLuint *tex = pTexture->castData<GLuint>();
	if (!tex)
		return DrawMissingImage(rect);

	//drawn non texture interactions
	if (m_lastDraw != LD_TEXTURE_RECT)
		Flush();

	GLuint boundtex;
	GLboolean texturesOn;
	glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundtex);
	GetGLErrName();

	if (!texturesOn || *tex != boundtex)
	{
		Flush();
		BindTexture(pTexture);
	}
	
	Translate(rect);
		
	//resize to openGl view port size
	renderRect[0] = ((float)rect.x / (float)m_Width);
	renderRect[1] = ((float)rect.y / (float)m_Height);
	renderRect[2] = ((float)(rect.x + rect.w) / (float)m_Width);
	renderRect[3] = ((float)(rect.y + rect.h) / (float)m_Height);
	renderRect[1] *= -1;
	renderRect[3] *= -1;

	AddVert(renderRect[0], renderRect[1], u1, v1); //BL
	AddVert(renderRect[2], renderRect[1], u2, v1); //BR
	AddVert(renderRect[2], renderRect[3], u2, v2); //TR
	AddVert(renderRect[0], renderRect[3], u1, v2); //TL

	m_lastDraw = LD_TEXTURE_RECT;
}

void Gwen::Renderer::OpenGL::LoadTexture(Gwen::Texture* pTexture)
{
	//Load image at specified path		
	SDL_Surface* loadedSurface = SDL_LoadBMP(pTexture->getName().c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", pTexture->getName().c_str(), SDL_GetError());
		return;
	}

	//get 
	LoadTexture(pTexture, loadedSurface->w, loadedSurface->h,
		loadedSurface->format->BytesPerPixel, (char*)loadedSurface->pixels);

	//free the surface
	SDL_free(loadedSurface);
	loadedSurface = nullptr;
}

void Gwen::Renderer::OpenGL::LoadTexture(Gwen::Texture * pTexture, Uint32 w, Uint32 h, Uint8 bits, const char * col)
{
	GLenum format;
	GLuint* tex = new GLuint;
	pTexture->getWidth() = w;
	pTexture->getHeight() = h;
	pTexture->getBitPP() = bits;
	pTexture->getData() = tex;	//set the pointer location

	if (bits == 1)			//grayscale
		format = GL_R;
	else if (bits == 3)	//color
		format = GL_RGB;
	else if (bits == 4)	//color alpha
		format = GL_RGBA;

	// Create the opengl texture
	glGenTextures(1, pTexture->castData<GLuint>());
	GetGLErrName();

	//set the texture data
	BindTexture(pTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTexture->getWidth(), pTexture->getHeight(), 0, format, GL_UNSIGNED_BYTE, (const GLvoid*)col);

	GetGLErrName();
	UnbindTexture();
}

void Gwen::Renderer::OpenGL::FreeTexture(Gwen::Texture* pTexture)
{
	GLuint* tex = pTexture->castData<GLuint>();

	if (!tex)
		return;

	glDeleteTextures(1, tex);
	delete tex;
	pTexture->getData() = nullptr;
}

Gwen::Color Gwen::Renderer::OpenGL::PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default)
{
	Gwen::Color c;
	if (!pTexture)
		return col_default;

	//image not loaded
	if (!pTexture->getData())
		return col_default;

	const GLuint texId = *pTexture->castData<GLuint>();
	if (!pTexture->getData())
		return col_default;
	

	//out of image bounds
	if (x > pTexture->getWidth() || y > pTexture->getHeight())
		return col_default;
#if 1 //waste of time and memory
	unsigned int iPixelSize = sizeof(unsigned char) * 4;
	glBindTexture(GL_TEXTURE_2D, texId);
	unsigned char* data = (unsigned char*)malloc(iPixelSize * pTexture->getWidth() * pTexture->getHeight());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	unsigned int iOffset = (y * pTexture->getWidth() + x) * 4;

	c.r = data[0 + iOffset];
	c.g = data[1 + iOffset];
	c.b = data[2 + iOffset];
	c.a = data[3 + iOffset];
	//
	// Retrieving the entire texture for a single pixel read
	// is kind of a waste - maybe cache this pointer in the texture
	// data and then release later on? It's never called during runtime
	// - only during initialization.
	//
	free(data);
#else
	// Retrieving the entire texture for a single pixel read
	// is kind of a waste - maybe cache this pointer in the texture
	// data and then release later on? It's never called during runtime
	// - only during initialization.
	GLenum format;
	Uint16 depth;
	depth = pTexture->getBitPP();

	if (depth == 1)			//grayscale
		format = GL_R;
	else if (depth == 3)	//color
		format = GL_RGB;
	else if (depth == 4)	//color alpha
		format = GL_RGBA;

	size_t pixelSize = pTexture->getBitPP() * sizeof(unsigned char);
	void* pixel = SDL_malloc(pixelSize + 1);
	if (!pixel)
		return col_default;

	BindTexture(pTexture);
	
	//get a single pixel in the image at determined position
	glGetTextureSubImage(texId, 0, x, y, 0, 1, 1, 0, format, GL_UNSIGNED_BYTE, pixelSize, pixel);
	GetGLErrName();
	UnbindTexture();

	if (depth == 1)			//grayscale
		c.r = c.g = c.b = c.a = *(char*)pixel;
	else if (depth == 3)	//color
	{
		c.r = ((char*)pixel)[0];
		c.g = ((char*)pixel)[1];
		c.b = ((char*)pixel)[2];
		c.a = 0xff;
	}
	else if (depth == 4)	//color alpha
	{
		c.r = ((char*)pixel)[0];
		c.g = ((char*)pixel)[1];
		c.b = ((char*)pixel)[2];
		c.a = ((char*)pixel)[3];
	}

	SDL_free(pixel);
	
#endif
	return c;
}

void Gwen::Renderer::OpenGL::LoadFont(Gwen::Font::FontBase * pFont)
{
	if (!pFont)
		return;

	//texture that store the font atlas 
	Gwen::Texture* pTexture = new Gwen::Texture;
	if (!pTexture)
		return;

	LoadTexture(pTexture, pFont->GetAtlasWidth(), pFont->GetAtlasWidth(), pFont->GetAtlasColorDept(), 
		(const char*)pFont->GetAtlasBuff());

	pFont->GetData() = pTexture;
}

void Gwen::Renderer::OpenGL::FreeFont(Gwen::Font::FontBase * pFont)
{
	if (!pFont)
		return;

	Gwen::Texture* 	pTexture = pFont->castData<Gwen::Texture>();
	if (!pTexture)
		return;

	FreeTexture(pTexture);
	delete pTexture;
}

static const float g_tabStops[4] = { 150, 210, 270, 330 };
void Gwen::Renderer::OpenGL::RenderText(Gwen::Font::FontBase * pFont, Gwen::Point pos, const Gwen::UnicodeString & string)
{
	if (!pFont)
		return;

	Gwen::Texture* pTexture = pFont->castData<Gwen::Texture>();
	if (!pTexture)
		return;

	// Missing image, not loaded properly?
	GLuint tex = *pTexture->castData<GLuint>();
	if (!tex)
		return;

	//drawn non texture interactions
	if (m_lastDraw != LD_TEXT_RECT)
		Flush();

	GLuint boundtex;
	GLboolean texturesOn;
	glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundtex);
	GetGLErrName();

	if (!texturesOn || tex != boundtex)
	{
		Flush();
		BindTexture(pTexture);
	}

	float posX = (float)pos.x;
	float posY = (float)pos.y;

	const float ox = pos.x;
	const char* text = Gwen::Utility::UnicodeToString(string).c_str();
	while (*text)
	{
		int c = (unsigned char)*text;
		if (c == '\t')
		{
			for (int i = 0; i < 4; ++i)
			{
				if (pos.x < g_tabStops[i] + ox)
				{
					pos.x = g_tabStops[i] + ox;
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{
			Font::CharRect FontRect;
			pFont->GetCharRenderRect(c - 32, posX, posY, FontRect);

			AddVert(FontRect.x, FontRect.y, FontRect.s0, FontRect.t0);
			AddVert(FontRect.w, FontRect.h, FontRect.s1, FontRect.t1);
			AddVert(FontRect.w, FontRect.y, FontRect.s1, FontRect.t0);
			AddVert(FontRect.x, FontRect.y, FontRect.s0, FontRect.t0);
			AddVert(FontRect.x, FontRect.h, FontRect.s0, FontRect.t1);
			AddVert(FontRect.w, FontRect.h, FontRect.s1, FontRect.t1);
		}
		++text;
	}
	m_lastDraw = LD_TEXT_RECT;
}

Gwen::Point Gwen::Renderer::OpenGL::MeasureText(Gwen::Font::FontBase * pFont, const Gwen::UnicodeString & text)
{
	Gwen::Point size;
	if (pFont)
		return size;

	size.x = pFont->GetTextLength(text);
	size.y = pFont->Size();
	return size;
}

void  Gwen::Renderer::OpenGL::BindTexture(Gwen::Texture* pTexture)
{
	if (!pTexture->getData())
		return;

	GLuint texId = *pTexture->castData<GLuint>();	//to help debug
	GLboolean texturesOn;
	glGetBooleanv(GL_TEXTURE_2D, &texturesOn);

	if (!texturesOn)
		glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void  Gwen::Renderer::OpenGL::UnbindTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);

	GLboolean texturesOn;
	glGetBooleanv(GL_TEXTURE_2D, &texturesOn);

	if (texturesOn)
		glDisable(GL_TEXTURE_2D);
}

bool Gwen::Renderer::OpenGL::InitializeContext(Gwen::WindowProvider* pWindow)
{
	SDL_Window *m_RenderWindow = static_cast<SDL_Window*>(pWindow->GetWindow());
	if (!m_RenderWindow)
		return false;

	// Create an OpenGL context associated with the window.
	m_pContext = (void*) SDL_GL_CreateContext(m_RenderWindow);
	if (!m_pContext)
		return false;

	if (glewInit() != GLEW_OK)
		return false;

	SDL_GL_SetSwapInterval(1);
	SDL_GL_MakeCurrent(m_RenderWindow, m_pContext);

	int w, h;
	SDL_GetWindowSize(m_RenderWindow, &w, &h);
	glViewport(w * -1, 0, w *2 , h * 2);
	m_Width = w;
	m_Height = h;
	
	return true;
}

bool Gwen::Renderer::OpenGL::ShutdownContext(Gwen::WindowProvider* pWindow)
{
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	if (m_pContext)
		SDL_GL_DeleteContext(m_pContext);

	return true;
}

bool Gwen::Renderer::OpenGL::PresentContext(Gwen::WindowProvider* pWindow)
{
	SDL_Window *m_RenderWindow = static_cast<SDL_Window*>(pWindow->GetWindow());
	if (!m_RenderWindow)
		return false;

	//present the context render 
	SDL_GL_SwapWindow(m_RenderWindow);
	return true;
}

bool Gwen::Renderer::OpenGL::ResizedContext(Gwen::WindowProvider* pWindow, int w, int h)
{
	SDL_Window *m_RenderWindow = static_cast<SDL_Window*>(pWindow->GetWindow());
	if (!m_RenderWindow)
		return false;

	SDL_GetWindowSize(m_RenderWindow, &w, &h);
	glViewport(w * -1, 0, w * 2, h * 2);
	m_Width = w;
	m_Height = h;

	return true;
}

bool Gwen::Renderer::OpenGL::BeginContext(Gwen::WindowProvider* pWindow)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}

bool Gwen::Renderer::OpenGL::EndContext(Gwen::WindowProvider* pWindow)
{
	return true;
}