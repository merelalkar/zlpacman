#ifndef PEGAS_PLATFORM_CONCRETE_RESOURCES_H
#define PEGAS_PLATFORM_CONCRETE_RESOURCES_H
#pragma once

namespace Pegas
{
	struct IntResourceCode
	{
	public:
		IntResourceCode()
			:module(0), resourceID(0)
		{

		};

		IntResourceCode(HINSTANCE _module, UINT _resourceID)
			:module(_module), resourceID(_resourceID)
		{
			
		};

		HINSTANCE module;
		UINT resourceID;
	};

#define MAKE_INT_RESOURCE_CODE(module, id) Pegas::IntResourceCode((module), (id))  

	class StringResource: public BaseResource<IntResourceCode>
	{
	public:
		enum
		{
			k_bufferLength = 512
		};

		StringResource() {};
		virtual ~StringResource() {};

		virtual void create(const IntResourceCode& code);
		virtual void load();
		virtual void destroy();

		String& getString() { return m_loadedString; };

	private:
		StringResource(const StringResource& src);
		StringResource& operator=(const StringResource& src);

		String m_loadedString;
	};

	typedef ResourceManager<StringResource, IntResourceCode> StringResourceManager;

	class SoundResource: public BaseResource<IntResourceCode>
	{
	public:
		SoundResource() {};
		virtual ~SoundResource() {};

		virtual void create(const IntResourceCode& code)
		{
			m_resourceCode = code;
			m_izInitialized = true;
		};

		virtual void load() {};
		virtual void destroy() {};

		IntResourceCode& getCode() { return m_resourceCode; };

	private:
		SoundResource(const SoundResource& src);
		SoundResource& operator=(const SoundResource& src);
	};

	typedef ResourceManager<SoundResource, IntResourceCode> SoundResourceManager;

	class TextureResource: public BaseResource<IntResourceCode>
	{
	public:
		enum
		{
			k_colorKey = 0x00000000
		};

		TextureResource();
		virtual ~TextureResource();

		virtual void create(const IntResourceCode& code);
		virtual void load();
		virtual void destroy();

		DWORD* getImageBits() const { return m_pPixels; };
		DWORD* getInvertedImageBits() const { return m_pInvertedPixels; };
		int getImageWidth() const { return m_nImageWidth; };
		int getImageHeight() const { return m_nImageHeght; };

		GLuint getTexture() const { return m_textureID; };
		GLuint getInvertedTexture() const { return m_invertedTextureID; };
		GLfloat getMaxUCoord() const { return m_fMaxUCoord; };
		GLfloat getMaxVCoord() const { return m_fMaxVCoord; };

	protected:
		TextureResource(const TextureResource& src);
		TextureResource& operator=(const TextureResource& src);

		static int s_instanceCounter;
		static HDC s_deviseContext;
		static std::set<int>* s_pTextureValidSizes;

		int inreaseInstanceCounter();
		int decreaseInstanceCounter();
		HDC& getDeviceContext() { return s_deviseContext; };
		
		PBITMAPINFO createBitmapInfoStruct(HBITMAP hBmp, int& nPitch);
		GLuint createTexture(DWORD* pBuffer, int width, int height);

		DWORD* m_pPixels;
		DWORD* m_pInvertedPixels;
		int m_nImageWidth;
		int m_nImageHeght;

		GLuint m_textureID;
		GLuint m_invertedTextureID;
		GLfloat m_fMaxUCoord;
		GLfloat m_fMaxVCoord;
	};

	typedef ResourceManager<TextureResource, IntResourceCode> TextureResourceManager;

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

		virtual void create(const FontResourceCode& code);
		virtual void load();
		virtual void destroy();

		GLuint getFontID() { return m_dlBaseOffset; };
		GLubyte* prepareString(const String& text, int& bufferLength);
		void getTextExtent(const String& text, CURCOORD& width, CURCOORD& height);

	private:
		FontResource(const FontResource& src);
		FontResource& operator=(const FontResource& src);

		static int s_instanceCounter;
		static HDC s_deviseContext;
		
		int inreaseInstanceCounter();
		int decreaseInstanceCounter();
		HDC& getDeviceContext() { return s_deviseContext; };
		
		HFONT m_hFont;
		GLuint m_dlBaseOffset;
		GLubyte m_buffer[k_maxBufferLength];		
	};

	typedef ResourceManager<FontResource, FontResourceCode> FontResourceManager; 

	
}

#endif //PEGAS_PLATFORM_CONCRETE_RESOURCES_H