/*
 * GraphicsTexture.h
 *
 *  Created on: 11.08.2014
 *      Author: DNS
 */

#pragma once

#include "packt_types.h"
#include "context.h"
#include "resource.h"

#include <android_native_app_glue.h>
#include <GLES/gl.h>
#include <png.h>

namespace packt
{
	class GraphicsTexture
	{
	public:
		GraphicsTexture(android_app* app, const char* path);
		~GraphicsTexture();

		int32_t getWidth();
		int32_t getHeight();
		const char* getPath();

		status load();
		void unload();
		void apply();

	protected:
		uint8_t* loadImage();

	private:
		static void callback_read(png_structp pStruct, png_bytep pData, png_size_t pSize);

		Resource mResource;
		GLuint mTextureId;
		GLuint mFormat;
		int32_t mWidth;
		int32_t mHeight;
	};
}


