#include "platform_windows.h"
#include "ogl_texture.h"

namespace Pegas
{
	OGLTexture::OGLTexture()
		:m_textureID(GL_INVALID_VALUE),
		m_width(0),
		m_height(0)
	{

	}

	OGLTexture::~OGLTexture()
	{
		destroy();
	}

	bool OGLTexture::create(int32 width, int32 height, int32 format, int32 flags)
	{
		m_width = width;
		m_height = height;

		// The texture we're going to render to
		glGenTextures(1, &m_textureID);
 
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, m_textureID);
 
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
 
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		return true;
	}

	void OGLTexture::destroy()
	{
		//TODO:
	}

	void OGLTexture::apply()
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	int32 OGLTexture::width()
	{
		return m_width;
	}

	int32 OGLTexture::height()
	{
		return m_height;
	}
}