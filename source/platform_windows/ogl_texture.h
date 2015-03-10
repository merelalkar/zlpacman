#pragma once

#include "texture.h"

namespace Pegas
{
	class OGLTexture: public Texture
	{
	public:
		OGLTexture();
		~OGLTexture();

		virtual bool create(int32 width, int32 height, 
			int32 format = -1, int32 flags = 0);
		virtual void destroy();
		virtual void apply();

		virtual int32 width();
		virtual int32 height();		
	private:
		GLuint m_textureID;
		int32 m_width;
		int32 m_height;
	};
}