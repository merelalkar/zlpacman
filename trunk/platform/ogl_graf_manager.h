#ifndef PEGAS_PLATFORM_OGL_GRAF_MANAGER_H
#define	PEGAS_PLATFORM_OGL_GRAF_MANAGER_H
#pragma once

namespace Pegas
{
	class EllipseTexture: public TextureResource
	{
	public:
		enum
		{
			k_textureWidth = 64,
			k_textureHeight = 64
		};

		EllipseTexture() {};
		virtual ~EllipseTexture() {};

		virtual void create(const IntResourceCode& code) {};
		virtual void load();
		
		DWORD* getEllipseBorderBits() const { return getImageBits(); }
		DWORD* getEllipseContentBits() const { return getInvertedImageBits(); };
		GLuint getEllipseBorderTexture() const { return getTexture(); };
		GLuint getEllipseContentTexture() const { return getInvertedTexture(); }
	};

	class GMException: public std::exception
	{
	public:
		GMException() {};
		GMException(const String& message) {};
	};

	class OGLGrafManager: public GrafManager
	{
	public:
		OGLGrafManager():m_window(0), m_deviceContext(0), m_glRenderContext(0) { };		

		void initialize(HWND window);
		void setViewport(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height);
		virtual void destroy();
		virtual void render();

		virtual void beginScene();
		virtual void endScene();

		virtual CURCOORD getCanvasWidth();
		virtual CURCOORD getCanvasHeight();

		virtual void drawLine(CURCOORD fromX, CURCOORD fromY, CURCOORD toX, CURCOORD toY, RGBCOLOR color);
		virtual void drawRectangle(CURCOORD left, CURCOORD top, CURCOORD width, 
			CURCOORD height, RGBCOLOR border, RGBCOLOR fill);
		virtual void drawEllipse(CURCOORD left, CURCOORD top, CURCOORD width, 
			CURCOORD height, RGBCOLOR border, RGBCOLOR fill);		
		virtual void drawSprite(const SpriteParameters& params);
		
		virtual void drawText(const String& text, const TextParameters& params);
		virtual void drawText(RESOURCEID textID, const TextParameters& params);
		virtual void getTextExtent(const String& text, RESOURCEID fontID, CURCOORD& width, CURCOORD& height);
		virtual void getTextExtent(RESOURCEID textID, RESOURCEID fontID, CURCOORD& width, CURCOORD& height);


	private:
		HWND  m_window;
		HDC   m_deviceContext;
		HGLRC m_glRenderContext;

		EllipseTexture m_ellipseTexture;

		OGLGrafManager(const OGLGrafManager& src);
		OGLGrafManager& operator=(const OGLGrafManager& src);
	};

	
}

#endif