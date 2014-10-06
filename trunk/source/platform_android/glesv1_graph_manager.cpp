/*
 * glesv1_graph_manager.cpp
 *
 *  Created on: 06.10.2014
 *      Author: borisov.v
 */
#include "glesv1_graph_manager.h"
#include "log.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace Pegas
{
	bool OGLGrafManager::initialize(ANativeWindow* window)
	{
		Pegas_log_info("GraphicsService::start");

		EGLint format, numConfig, errorResult;
		EGLConfig config;
		const EGLint attributes[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_NONE
		};

		mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(mDisplay == EGL_NO_DISPLAY)
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("mDisplay == EGL_NO_DISPLAY");

			errorReport("eglGetDisplay");
			destroy();

			return true;
		}

		if(!eglInitialize(mDisplay, NULL, NULL))
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("!eglInitialize");
			Pegas_log_warning("mDisplay = %d [%X]", mDisplay, mDisplay);

			errorReport("eglInitialize");
			destroy();

			return false;
		}

		if(!eglChooseConfig(mDisplay, attributes, &config, 1, &numConfig))
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("!eglChooseConfig");
			Pegas_log_warning("mDisplay = %d [%X]", mDisplay, mDisplay);

			errorReport("eglChooseConfig");
			destroy();

			return false;
		}

		if(!eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format))
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("!eglGetConfigAttrib");
			Pegas_log_warning("mDisplay = %d [%X]", mDisplay, mDisplay);
			Pegas_log_warning("config = %d [%X]", config, config);

			errorReport("eglGetConfigAttrib");
			destroy();

			return false;
		}

		ANativeWindow_setBuffersGeometry(window, 0, 0, format);

		mSurface = eglCreateWindowSurface(mDisplay, config, window, NULL);
		if(mSurface == EGL_NO_SURFACE)
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("eglCreateWindowSurface == EGL_NO_SURFACE");
			Pegas_log_warning("mDisplay = %d [%X]", mDisplay, mDisplay);
			Pegas_log_warning("config = %d [%X]", config, config);
			Pegas_log_warning("mApplication->window = %X", window);

			errorReport("eglCreateWindowSurface");
			destroy();

			return false;
		}

		mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, NULL);
		if(mContext == EGL_NO_CONTEXT)
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("eglCreateContext == EGL_NO_CONTEXT");
			Pegas_log_warning("mDisplay = %d [%X]", mDisplay, mDisplay);
			Pegas_log_warning("config = %d [%X]", config, config);

			errorReport("eglCreateContext");
			destroy();

			return false;
		}

		int32_t width, height;
		EGLBoolean makeCurrent = eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
		EGLBoolean querySurfaceWidth = eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &width);
		EGLBoolean querySurfaceHeight = eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &height);
		if(!makeCurrent || !querySurfaceWidth || !querySurfaceHeight || (width <= 0) ||  (height <= 0))
		{
			Pegas_log_warning("GraphicsService::start:");
			Pegas_log_warning("makeCurrent = %d", makeCurrent);
			Pegas_log_warning("querySurfaceWidth = %d", querySurfaceWidth);
			Pegas_log_warning("querySurfaceHeight = %d", querySurfaceHeight);
			Pegas_log_warning("mWidth = %d", width);
			Pegas_log_warning("mHeight = %d", weight);


			errorReport("eglMakeCurrent or eglQuerySurface");
			destroy();

			return false;
		}

		mWidth = width;
		mHeight = height;

		setup();

		return true;
	}

	void GLESv1_GraphManager::destroy()
	{
		Pegas_log_info("GraphicsService::stop");

		if (mDisplay != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

			if (mContext != EGL_NO_CONTEXT)
			{
				eglDestroyContext(mDisplay, mContext);
				mContext = EGL_NO_CONTEXT;
			}

			if (mSurface != EGL_NO_SURFACE)
			{
				eglDestroySurface(mDisplay, mSurface);
				mSurface = EGL_NO_SURFACE;
			}

			eglTerminate(mDisplay);
			mDisplay = EGL_NO_DISPLAY;
		}
	}

	void GLESv1_GraphManager::setup()
	{
		Pegas_log_info("GraphicsService::setup");

		glViewport(0, 0, mWidth, mHeight);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0.0f, mWidth, 0.0f, mHeight, 0.0f, 1.0f);

		glMatrixMode( GL_MODELVIEW);
		glLoadIdentity();
	}

	void GLESv1_GraphManager::errorReport(const char* message)
	{
		Pegas_log_info("GraphicsService::errorReport [message: %s]", message);
		Pegas_log_error("Error on calling function: %s", message);
		EGLint code = eglGetError();

		switch(code)
		{
		case EGL_NOT_INITIALIZED:
			Pegas_log_error("Error code: EGL_NOT_INITIALIZED");
			break;
		case EGL_BAD_ACCESS:
			Pegas_log_error("Error code: EGL_BAD_ACCESS");
			break;
		case EGL_BAD_ALLOC:
			Pegas_log_error("Error code: EGL_BAD_ALLOC");
			break;
		case EGL_BAD_ATTRIBUTE:
			Pegas_log_error("Error code: EGL_BAD_ATTRIBUTE");
			break;
		case EGL_BAD_CONFIG:
			Pegas_log_error("Error code: EGL_BAD_CONFIG");
			break;
		case EGL_BAD_CONTEXT:
			Pegas_log_error("Error code: EGL_BAD_CONTEXT");
			break;
		case EGL_BAD_CURRENT_SURFACE:
			Pegas_log_error("Error code: EGL_BAD_CURRENT_SURFACE");
			break;
		case EGL_BAD_DISPLAY:
			Pegas_log_error("Error code: EGL_BAD_DISPLAY");
			break;
		case EGL_BAD_MATCH:
			Pegas_log_error("Error code: EGL_BAD_MATCH");
			break;
		case EGL_BAD_NATIVE_PIXMAP:
			Pegas_log_error("Error code: EGL_BAD_NATIVE_PIXMAP");
			break;
		case EGL_BAD_NATIVE_WINDOW:
			Pegas_log_error("Error code: EGL_BAD_NATIVE_WINDOW");
			break;
		case EGL_BAD_PARAMETER:
			Pegas_log_error("Error code: EGL_BAD_PARAMETER");
			break;
		case EGL_BAD_SURFACE:
			Pegas_log_error("Error code: EGL_BAD_SURFACE");
			break;
		case EGL_CONTEXT_LOST:
			Pegas_log_error("Error code: EGL_CONTEXT_LOST");
			break;
		default:
			break;
		}
	}

	void GLESv1_GraphManager::beginScene()
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GLESv1_GraphManager::endScene()
	{
		if(!eglSwapBuffers(mDisplay, mSurface))
		{
			Pegas_log_warning("GraphicsService::update:");
			Pegas_log_warning("!eglSwapBuffers");
			Pegas_log_warning("mDisplay = %d [%X]", mDisplay, mDisplay);
			Pegas_log_warning("mSurface = %d [%X]", mSurface, mSurface);

			errorReport("eglSwapBuffers");
			destroy();
		}
	}

	void GLESv1_GraphManager::drawLine(CURCOORD fromX, CURCOORD fromY, CURCOORD toX, CURCOORD toY, RGBCOLOR color)
	{
		glColor4ub(((color & 0x00ff0000) >> 16), ((color & 0x0000ff00) >> 8),
				(color & 0x000000ff), ((color & 0xff000000) >> 24));

		glBegin(GL_LINES);
		glVertex2f(fromX, fromY);
		glVertex2f(toX, toY);
		glEnd();
	}

	void GLESv1_GraphManager::drawRectangle(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, RGBCOLOR border, RGBCOLOR fill)
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

	void GLESv1_GraphManager::drawEllipse(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, RGBCOLOR border, RGBCOLOR fill)
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
		//glBindTexture(GL_TEXTURE_2D, m_ellipseTexture.getEllipseBorderTexture());

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(left, top);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(left + width, top);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(left + width, top + height);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(left, top + height);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	void GLESv1_GraphManager::drawSprite(const SpriteParameters& params)
	{

	}

	void GLESv1_GraphManager::drawText(const String& text, const TextParameters& params)
	{

	}

	void GLESv1_GraphManager::drawText(RESOURCEID textID, const TextParameters& params)
	{

	}

	void GLESv1_GraphManager::getTextExtent(const String& text, RESOURCEID fontID, CURCOORD& width, CURCOORD& height)
	{

	}

	void GLESv1_GraphManager::getTextExtent(RESOURCEID textID, RESOURCEID fontID, CURCOORD& width, CURCOORD& height)
	{

	}
}




