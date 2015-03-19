#include "platform_windows.h"
using namespace Pegas;

OGLGrafManager::OGLGrafManager()
	:m_window(0), 
	 m_deviceContext(0), 
	 m_glRenderContext(0),
	 m_renderTarget(NULL),
	 m_screenLeft(0),
	 m_screenTop(0),
	 m_screenWidth(0),
	 m_screenHeight(0)
{

}	

void OGLGrafManager::initialize(HWND window)
{
	PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)) ;
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion   = 1 ; 
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW ;
	pfd.iPixelType = PFD_TYPE_RGBA; 
	pfd.cColorBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE ;
		
	m_deviceContext = GetDC(window);
	int nPixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
	if (nPixelFormat == 0)
	{
	  throw GMException(_text("ChoosePixelFormat Failed"));      
	}

	BOOL bResult = SetPixelFormat (m_deviceContext, nPixelFormat, &pfd);
	if (!bResult)
	{
	  throw GMException(_text("SetPixelFormat Failed"));      
	}

	m_glRenderContext = wglCreateContext(m_deviceContext);
	if (!m_glRenderContext)
	{
		throw GMException(_text("wglCreateContext Failed"));      
	}

	wglMakeCurrent(m_deviceContext, m_glRenderContext);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	m_ellipseTexture.load();
	TextureResourceManager::getInstance().loadAll();
	FontResourceManager::getInstance().loadAll();
	
}

void OGLGrafManager::setViewport(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height)
{
	m_screenLeft = (GLint)left;
	m_screenTop = (GLint)top;
	m_screenWidth = (GLsizei)width;
	m_screenHeight = (GLsizei)height;
	
	_setViewport(m_screenLeft, m_screenTop, m_screenWidth, m_screenHeight);
}

void OGLGrafManager::_setViewport(GLint left, GLint top, GLsizei width, GLsizei height)
{
	if(!wglMakeCurrent(m_deviceContext, m_glRenderContext))
	{
		throw GMException(_text("wglMakeCurrent Failed"));
	}

	glViewport(left, top, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, width, height, top);
}

void OGLGrafManager::destroy()
{
	m_ellipseTexture.destroy();

	if(m_glRenderContext)
		wglDeleteContext(m_glRenderContext);
	
	if(m_deviceContext)
		ReleaseDC(m_window, m_deviceContext);
	
    m_window = 0;
	m_deviceContext = 0;
	m_glRenderContext = 0;
}

void OGLGrafManager::render()
{
	beginScene();

	for(DrawableLayoutsList::iterator it = m_layouts.begin();
		it != m_layouts.end(); ++it)
	{
		(*it)->draw(*this);
	}

	endScene();	
}

void OGLGrafManager::beginScene(Texture* texture)
{
	wglMakeCurrent(m_deviceContext, m_glRenderContext);

	//TODO: http://masandilov.ru/opengl/render-to-texture
	m_renderTarget = texture;
	if(m_renderTarget)
	{
		_setViewport(0, 0, m_renderTarget->width(), m_renderTarget->height());
	}else
	{
		//TODO:
		_setViewport(m_screenLeft, m_screenTop, m_screenWidth, m_screenHeight);
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);	
}

void OGLGrafManager::endScene()
{
	//TODO: http://masandilov.ru/opengl/render-to-texture
	if(m_renderTarget)
	{
		m_renderTarget->apply();

		 glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 
			 m_renderTarget->width(), m_renderTarget->height(), 0);
		 glClear(GL_COLOR_BUFFER_BIT);

		m_renderTarget = NULL;
	}else
	{
		glFlush();
		SwapBuffers(m_deviceContext);
	}
}

CURCOORD OGLGrafManager::getCanvasWidth()
{
	GLfloat values[4];
	glGetFloatv(GL_VIEWPORT, values);

	return values[2];
}

CURCOORD OGLGrafManager::getCanvasHeight()
{
	GLfloat values[4];
	glGetFloatv(GL_VIEWPORT, values);

	return values[3];
}

void OGLGrafManager::drawLine(CURCOORD fromX, CURCOORD fromY, CURCOORD toX, CURCOORD toY, 
							  RGBCOLOR color)
{
	glColor4ub(((color & 0x00ff0000) >> 16), ((color & 0x0000ff00) >> 8), 
		(color & 0x000000ff), ((color & 0xff000000) >> 24));
	
	glBegin(GL_LINES);
	glVertex2f(fromX, fromY);
	glVertex2f(toX, toY);
	glEnd();
}

void OGLGrafManager::drawRectangle(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, 
								   RGBCOLOR border, RGBCOLOR fill)
{
	glColor4ub(((fill & 0x00ff0000) >> 16), ((fill & 0x0000ff00) >> 8), 
		(fill & 0x000000ff), ((fill & 0xff000000) >> 24));

	glBegin(GL_QUADS);
	glVertex2f(left, top);
	glVertex2f(left + width, top);
	glVertex2f(left + width, top + height);
	glVertex2f(left, top + height);
	glEnd();

	if(border == fill) return;

	glColor4ub(((border & 0x00ff0000) >> 16), ((border & 0x0000ff00) >> 8), 
		(border & 0x000000ff), ((border & 0xff000000) >> 24));

	glBegin(GL_LINE_LOOP);
	glVertex2f(left, top);
	glVertex2f(left + width, top);
	glVertex2f(left + width, top + height);
	glVertex2f(left, top + height);
	glEnd();
}

void OGLGrafManager::drawEllipse(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, 
								 RGBCOLOR border, RGBCOLOR fill)
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(((fill & 0x00ff0000) >> 16), ((fill & 0x0000ff00) >> 8), 
		(fill & 0x000000ff), ((fill & 0xff000000) >> 24));
	glBindTexture(GL_TEXTURE_2D, m_ellipseTexture.getEllipseContentTexture());

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(left, top);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(left + width, top);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(left + width, top + height);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(left, top + height);
	glEnd();

	if(border == fill)
	{
		glDisable(GL_TEXTURE_2D);
		return;
	}

	glColor4ub(((border & 0x00ff0000) >> 16), ((border & 0x0000ff00) >> 8), 
		(border & 0x000000ff), ((border & 0xff000000) >> 24));
	glBindTexture(GL_TEXTURE_2D, m_ellipseTexture.getEllipseBorderTexture());

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(left, top);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(left + width, top);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(left + width, top + height);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(left, top + height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void OGLGrafManager::drawSprite(const SpriteParameters& params)
{
	TextureResource* pTexture = TextureResourceManager::getInstance()
		.getResource(params._texture);
	
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255, 255, 255, 255);

	if((params._flags & k_invertTextureColors) == k_invertTextureColors)
	{
		glBindTexture(GL_TEXTURE_2D, pTexture->getInvertedTexture());
	}else
	{
		glBindTexture(GL_TEXTURE_2D, pTexture->getTexture());
	}
	GLfloat minU = 0;
	GLfloat minV = 0;
	GLfloat maxU = pTexture->getMaxUCoord();
	GLfloat maxV = pTexture->getMaxVCoord();
	/*
	GLint left = params._left;
	GLint top = params._top;
	GLint right = params._left + params._width;
	GLint bottom = params._top + params._height;*/

	if((params._flags & k_customTextureCoords) == k_customTextureCoords)
	{
		minU = params._minU;
		minV = params._minV;
		maxU = params._maxU;
		maxV = params._maxV;
	}else
	{
		if((params._flags & k_repeatTextureAlongX) == k_repeatTextureAlongX)
			maxU = (params._width * 1.0) / pTexture->getImageWidth();

		if((params._flags & k_repeatTextureAlongY) == k_repeatTextureAlongY)
			maxV = (params._height * 1.0) / pTexture->getImageHeight();
	}

	if(params._flags & k_flipHorizontal)
	{
		std::swap(minU, maxU);
	}

	if(params._flags &	k_flipVertical)
	{
		std::swap(minV, maxV);
	}

	glPushMatrix();
	glTranslatef((GLfloat)(params._left + (params._width * 0.5f)), 
		(GLfloat)(params._top + (params._height * 0.5f)), 0.0f);
	glRotatef(params._angle, 0.0f, 0.0f, 1.0f);
	glScalef((GLfloat)params._width, (GLfloat)params._height, 1.0f);

	glBegin(GL_QUADS);
	/*
	glTexCoord2f(minU, maxV); glVertex2i(left, top);
	glTexCoord2f(maxU, maxV); glVertex2i(right, top);
	glTexCoord2f(maxU, minV); glVertex2i(right, bottom);
	glTexCoord2f(minU, minV); glVertex2i(left, bottom);*/

	glTexCoord2f(minU, maxV); glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(maxU, maxV); glVertex2f(0.5f, -0.5f);
	glTexCoord2f(maxU, minV); glVertex2f(0.5f, 0.5f);
	glTexCoord2f(minU, minV); glVertex2f(-0.5f, 0.5f);

	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
		
void OGLGrafManager::drawText(const String& text, const TextParameters& params)
{
	FontResourceManager& manager = FontResourceManager::getInstance();
	FontResource* fontResource = manager.getResource(params._font);

	int bufferLength = 0;
	GLubyte* buffer = fontResource->prepareString(text, bufferLength);

	glColor4ub(((params._color & 0x00ff0000) >> 16), ((params._color & 0x0000ff00) >> 8), 
		(params._color & 0x000000ff), ((params._color & 0xff000000) >> 24));

	glRasterPos2f(params._left, params._top);
	glPushAttrib(GL_LIST_BIT);
	glListBase(fontResource->getFontID());
	glCallLists(bufferLength, GL_UNSIGNED_BYTE, buffer);
	glPopAttrib();
}

void OGLGrafManager::drawText(RESOURCEID textID, const TextParameters& params)
{
	StringResource* stringResource = StringResourceManager::getInstance().getResource(textID);
	drawText(stringResource->getString(), params);	
}

void OGLGrafManager::getTextExtent(const String& text, RESOURCEID fontID, CURCOORD& width, CURCOORD& height)
{
	FontResourceManager& manager = FontResourceManager::getInstance();
	FontResource* fontResource = manager.getResource(fontID);
	fontResource->getTextExtent(text, width, height);
}

void OGLGrafManager::getTextExtent(RESOURCEID textID, RESOURCEID fontID, CURCOORD& width, CURCOORD& height)
{
	StringResource* stringResource = StringResourceManager::getInstance().getResource(textID);
	getTextExtent(stringResource->getString(), fontID, width, height);	
}

TexturePtr OGLGrafManager::createTexture(int32 width, int32 height, int32 format, int32 flags)
{
	TexturePtr result(new OGLTexture());
	result->create(width, height, format, flags);

	return result;
}

/*************************************************************************************
		EllipseTexture class implementation
**************************************************************************************/

void EllipseTexture::load()
{
	destroy();

	m_nImageWidth = k_textureWidth;
	m_nImageHeght = k_textureHeight;
	m_pPixels = new DWORD[m_nImageWidth * m_nImageHeght];
	m_pInvertedPixels = new DWORD[m_nImageWidth * m_nImageHeght];

	DWORD* pBorderPointer = m_pPixels;
	DWORD* pContentPointer = m_pInvertedPixels;
	DWORD  r = min(m_nImageWidth, m_nImageHeght) / 2;
	DWORD  r2 = r * r;
	DWORD  delta = 1;
	DWORD  inner_r2 = (r - delta) * (r - delta);
	DWORD x0 =  m_nImageWidth / 2;
	DWORD y0 =  m_nImageHeght / 2;
	DWORD lx2, ly2;
	DWORD summ;

	for(int y = 0; y < m_nImageHeght; y++)
	{
		ly2 = (y - y0) * (y - y0);		
		for(int x = 0; x < m_nImageWidth; x++)
		{
			lx2 = (x - x0) * (x - x0);
			summ = lx2 + ly2;
            
			if(summ > r2)
			{
				(*pBorderPointer) = (*pContentPointer) = 0x00000000;
			}

			if(summ < r2 && summ >= inner_r2)
			{
				(*pBorderPointer) = 0xffffffff;
			}

			if(summ < r2)
			{
				(*pContentPointer) = 0xffffffff;				
			}

			
			if(summ < inner_r2)
			{
				(*pBorderPointer) = 0x00000000;
			}
			
			pContentPointer++;
			pBorderPointer++;
		}//for(int x = 0; x < m_nImageWidth; x++)
	}//for(int y = 0; y < m_nImageHeght; y++)

	m_fMaxUCoord = m_fMaxVCoord = 1.0;
	m_textureID = createTexture(m_pPixels, m_nImageWidth, m_nImageHeght);
	m_invertedTextureID = createTexture(m_pInvertedPixels, m_nImageWidth, m_nImageHeght);

}

