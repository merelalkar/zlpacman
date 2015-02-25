#ifndef PEGAS_PLATFORM_CONCRETE_RESOURCES_H
#define PEGAS_PLATFORM_CONCRETE_RESOURCES_H
#pragma once

#include "engine.h"

#include "packt_types.h"
#include "graphics_texture.h"

#include <android_native_app_glue.h>
#include <GLES/gl.h>

namespace Pegas
{
	struct PathResourceCode
	{
	public:
		PathResourceCode() {}
		PathResourceCode(android_app* app, const String& path)
		: _app(app), _path(path) {}

		android_app* _app;
		String _path;
	};

	#define MAKE_RESOURCE_CODE(app, path) Pegas::PathResourceCode(app, path)

	/****************************************************************************************************
	 *	Textures
	 * **************************************************************************************************/
	class TextureResource: public BaseResource<PathResourceCode>
	{
	public:
		enum
		{
			k_colorKey = 0x00000000
		};

		TextureResource();
		virtual ~TextureResource();

		virtual void create(const PathResourceCode& code);
		virtual void load();
		virtual void destroy();
		void apply();

		int32 getImageWidth() const { return m_isLoaded ? m_pTexture->getWidth() : 0; }
		int32 getImageHeight() const { return m_isLoaded ? m_pTexture->getHeight() : 0; }
		int32 getInnerImageWidth() const { return getImageWidth(); }
		int32 getInnerImageHeight() const { return getImageHeight(); }

		GLfloat getMaxUCoord() const { return 1.0f; }
		GLfloat getMaxVCoord() const { return 1.0f; }

	private:
		TextureResource(const TextureResource& src);
		TextureResource& operator=(const TextureResource& src);

	private:
		typedef SmartPointer<packt::GraphicsTexture> TexturePtr;
		TexturePtr m_pTexture;
		bool	   m_isLoaded;
	};

	typedef ResourceManager<TextureResource, PathResourceCode> TextureResourceManager;

	/***********************************************************************************************
	 * 	Fonts
	 * ********************************************************************************************/

	struct FontResourceCode
	{
		FontResourceCode(): fontSize(0) {};
		
		FontResourceCode(const String& name, FONTSIZE size)
			:fontName(name), fontSize(size)
		{ };
		
		String		fontName;
		FONTSIZE	fontSize;
	};

#define MAKE_FONT_RESOURCE_CODE(name, size) Pegas::FontResourceCode((name), (size)) 

	class FontResource: public BaseResource<FontResourceCode>
	{
	public:
		enum
		{
			k_numCharsInSet = 256,
			k_maxBufferLength = 1024
		};
		
		
		FontResource();
		virtual ~FontResource();

		virtual void create(const FontResourceCode& code) {}
		virtual void load() {}
		virtual void destroy() {}

		GLuint getFontID() { return m_dlBaseOffset; };
		GLubyte* prepareString(const String& text, int& bufferLength) { return 0; }
		void getTextExtent(const String& text, CURCOORD& width, CURCOORD& height);

	private:
		FontResource(const FontResource& src);
		FontResource& operator=(const FontResource& src);

		GLuint m_dlBaseOffset;
		GLubyte m_buffer[k_maxBufferLength];
	};

	typedef ResourceManager<FontResource, FontResourceCode> FontResourceManager; 

	
}

#endif //PEGAS_PLATFORM_CONCRETE_RESOURCES_H
