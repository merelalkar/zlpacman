/*
 * GraphicsTexture.cpp
 *
 *  Created on: 11.08.2014
 *      Author: DNS
 */
#include "Log.h"
#include "GraphicsTexture.h"

namespace Pegas
{


	GraphicsTexture::GraphicsTexture(android_app* app, const char* path)
		: mResource(app, path), mTextureId(0),
		  mFormat(0), mWidth(0), mHeight(0)
	{
		Pegas_log_info("GraphicsTexture constructor [app: %X, path: %s]", app, path);
	}

	GraphicsTexture::~GraphicsTexture()
	{
		Pegas_log_info("GraphicsTexture destructor");
	}

	int32_t GraphicsTexture::getWidth()
	{
		Pegas_log_info("GraphicsTexture::getWidth");

		return mWidth;
	}

	int32_t GraphicsTexture::getHeight()
	{
		Pegas_log_info("GraphicsTexture::getHeight");

		return mHeight;
	}

	const char* GraphicsTexture::getPath()
	{
		Pegas_log_info("GraphicsTexture::getPath");

		return mResource.getPath();
	}

	status GraphicsTexture::load()
	{
		Pegas_log_info("GraphicsTexture::load");

		uint8_t* imageBuffer = loadImage();
		if(imageBuffer == NULL)
		{
			return STATUS_KO;
		}

		//Creates a new OpenGL texture
		GLenum errorResult;
		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);

		//Set-up texture properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Loads image data into OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, imageBuffer);
		delete[] imageBuffer;

		if(glGetError() != GL_NO_ERROR)
		{
			Log::error("Error loading texture into OpenGL");
			unload();

			return STATUS_KO;
		}

		return STATUS_OK;
	}

	void GraphicsTexture::unload()
	{
		Pegas_log_info("GraphicsTexture::unload");

		if(mTextureId != 0)
		{
			glDeleteTextures(1, &mTextureId);
			mTextureId = 0;
		}

		mWidth = 0;
		mHeight = 0;
		mFormat = 0;
	}

	void GraphicsTexture::apply()
	{
		Pegas_log_info("GraphicsTexture::apply");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
	}

	uint8_t* GraphicsTexture::loadImage()
	{
		Pegas_log_info("GraphicsTexture::loadImage");

		Resource::AutoDispose autoDispose(mResource);
		if (mResource.open() != STATUS_OK)
		{
			Log::error("mResource.open failed");
			return NULL;
		}

		png_byte header[8];
		if (mResource.read(header, sizeof(header)) != STATUS_OK)
		{
			Log::error("mResource.read failed");
			return NULL;
		}

		if (png_sig_cmp(header, 0, 8) != 0)
		{
			Log::error("png_sig_cmp failed, header param: %s", header);
			return NULL;
		}

		png_structp pngMain = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!pngMain) {
			Log::error("png_create_read_struct failed");
			return NULL;
		}
		png_infop pngInfo = png_create_info_struct(pngMain);
		if (!pngInfo) {
			Log::error("png_create_info_struct failed");
			return NULL;
		}
		png_set_read_fn(pngMain, &mResource, callback_read);
		if (setjmp(png_jmpbuf(pngMain))) {
			Log::error("setjmp failed");
			return NULL;
		}
		png_set_sig_bytes(pngMain, 8);
		png_read_info(pngMain, pngInfo);
		png_int_32 depth, colorType;
		png_uint_32 width, height;
		png_get_IHDR(pngMain, pngInfo, &width, &height, &depth, &colorType, NULL,
				NULL, NULL);
		mWidth = width;
		mHeight = height;
		bool transparency = false;
		// Creates a full alpha channel if transparency is encoded as
		// an array of palette entries or a single transparent color.
		if (png_get_valid(pngMain, pngInfo, PNG_INFO_tRNS)) {
			png_set_tRNS_to_alpha(pngMain);
			transparency = true;

			Log::error("png_get_valid failed");
			return NULL;
		}
		// Expands PNG with less than 8bits per channel to 8bits.
		if (depth < 0) {
			png_set_packing(pngMain);
			// Shrinks PNG with 16bits per color channel down to 8bits.
		} else if (depth == 16) {
			png_set_strip_16(pngMain);
		}

		// Indicates that image needs conversion to RGBA if	needed.
		switch (colorType) {
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(pngMain);
			mFormat = transparency ? GL_RGBA : GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGB:
			mFormat = transparency ? GL_RGBA : GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGBA:
			mFormat = GL_RGBA;
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(pngMain);
			mFormat = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			break;
		case PNG_COLOR_TYPE_GA:
			png_set_expand_gray_1_2_4_to_8(pngMain);
			mFormat = GL_LUMINANCE_ALPHA;
			break;
		}
		png_read_update_info(pngMain, pngInfo);
		png_int_32 rowSize = png_get_rowbytes(pngMain, pngInfo);
		if (rowSize <= 0) {
			Log::error("invalid png row size: %d", rowSize);
			return NULL;
		}
		png_byte* imageBuffer = new png_byte[rowSize * height];
		if (!imageBuffer) {
			Log::error("can not allocate image buffer");
			return NULL;
		}
		png_bytep* rowPtrs = new png_bytep[height];
		if (!rowPtrs) {
			Log::error("can not allocate row pointers");
			//TODO: use smart pointer for this
			delete[] imageBuffer;

			return NULL;
		}
		for (int32_t i = 0; i < height; ++i) {
			rowPtrs[height - (i + 1)] = imageBuffer + (i * rowSize);
		}
		png_read_image(pngMain, rowPtrs);
		png_destroy_read_struct(&pngMain, &pngInfo, NULL);
		delete[] rowPtrs;

		return imageBuffer;
}



	void GraphicsTexture::callback_read(png_structp pStruct, png_bytep pData, png_size_t pSize)
	{
		Pegas_log_info("GraphicsTexture::callback_read [pStruct: %X, pData: %X, pSize: %d]", pStruct, pData, pSize);

		Resource& resourceReader = *((Resource*) (png_get_io_ptr(pStruct)));

		if(resourceReader.read(pData, pSize) != STATUS_OK)
		{
			resourceReader.close();
			png_error(pStruct, "Error while reading PNG file");
		}
	}
}
