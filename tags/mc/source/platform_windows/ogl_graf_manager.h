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
		OGLGrafManager();		

		void initialize(HWND window);
		void setViewport(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height);
		virtual void destroy();
		virtual void render();

		virtual void beginScene(Texture* texture = 0);
		virtual void endScene();

		virtual CURCOORD getCanvasWidth();
		virtual CURCOORD getCanvasHeight();

		virtual TexturePtr createTexture(int32 width, int32 height, int32 format = -1, int32 flags = 0);

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
		void _setViewport(GLint left, GLint top, GLsizei width, GLsizei height);

		HWND  m_window;
		HDC   m_deviceContext;
		HGLRC m_glRenderContext;
		
		GLint   m_screenLeft;
		GLint   m_screenTop;
		GLsizei m_screenWidth;
		GLsizei m_screenHeight;

		EllipseTexture m_ellipseTexture;
		Texture* m_renderTarget;

		OGLGrafManager(const OGLGrafManager& src);
		OGLGrafManager& operator=(const OGLGrafManager& src);
	};

	
}

#endif