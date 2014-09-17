/*
 * GraphicsService.cpp
 *
 *  Created on: 28.07.2014
 *      Author: DNS
 */
#include "GraphicsService.h"
#include "Log.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace Pegas
{
	GraphicsService::GraphicsService(android_app* app, TimeService* timer)
		:mApplication(app), mTimer(timer),
		 mWidth(0), mHeight(0),
		 mDisplay(EGL_NO_DISPLAY),
		 mSurface(EGL_NO_SURFACE),
		 mContext(EGL_NO_CONTEXT),
		 mTextureCount(0),
		 mSpriteCount(0),
		 mTileMapCount(0)
	{
		Pegas_log_debug("GraphicsService constructor [app: %X, timer: %X]", app, timer);
	}

	GraphicsService::~GraphicsService()
	{
		Pegas_log_debug("GraphicsService destructor");

		for(int32_t i = 0; i < mTextureCount; ++i)
		{
			delete mTextures[i];
			mTextures[i] = NULL;
		}
		mTextureCount = 0;

		for(int32_t i = 0; i < mSpriteCount; ++i)
		{
			delete mSprites[i];
			mSprites[i] = NULL;
		}
		mSpriteCount = 0;

		for(int32_t i = 0; i < mTileMapCount; ++i)
		{
			delete mTileMaps[i];
			mTileMaps[i] = NULL;
		}
		mTileMapCount = 0;
	}

	GraphicsTexture* GraphicsService::registerTexture(const char* pPath)
	{
		Pegas_log_debug("GraphicsService::registerTexture [pPath: %s]", pPath);

		for(int32_t i = 0; i < mTextureCount; ++i)
		{
			if(strcmp(pPath, mTextures[i]->getPath()) == 0)
			{
				return mTextures[i];
			}
		}

		GraphicsTexture* texture = new GraphicsTexture(mApplication, pPath);
		mTextures[mTextureCount++] = texture;

		return texture;
	}

	GraphicsSprite* GraphicsService::registerSprite(GraphicsTexture* texture, int32_t width, int32_t height)
	{
		Pegas_log_debug("GraphicsService::registerSprite [texture: %X, width: %d, height: %d]", texture, width, height);

		GraphicsSprite* sprite = new GraphicsSprite(texture, width, height);
		mSprites[mSpriteCount++] = sprite;

		return sprite;
	}

	GraphicsTileMap* GraphicsService::registerTileMap(const char* pPath, GraphicsTexture* pTexture, Location* pLocation)
	{
		Pegas_log_debug("GraphicsService::registerTileMap [pPath: %s, pTexture: %X, pLocation: %X]", pPath, pTexture, pLocation);

		GraphicsTileMap* tileMap = new GraphicsTileMap(mApplication, pPath, pTexture, pLocation);
		mTileMaps[mTileMapCount++] = tileMap;

		return tileMap;
	}

	void GraphicsService::setup()
	{
		Pegas_log_debug("GraphicsService::setup");

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0.0f, mWidth, 0.0f, mHeight, 0.0f, 1.0f);

		glMatrixMode( GL_MODELVIEW);
		glLoadIdentity();
	}

	status GraphicsService::loadResources()
	{
		Pegas_log_debug("GraphicsService::loadResources");

		for(int32_t i = 0; i < mTextureCount; ++i)
		{
			if(mTextures[i]->load() != STATUS_OK)
			{
				return STATUS_KO;
			}
		}

		for(int32_t i = 0; i < mSpriteCount; ++i)
		{
			mSprites[i]->load();
		}

		for(int32_t i = 0; i < mTileMapCount; ++i)
		{
			mTileMaps[i]->load();
		}

		return STATUS_OK;
	}

	status GraphicsService::unloadResources()
	{
		Pegas_log_debug("GraphicsService::unloadResources");

		for(int32_t i = 0; i < mTileMapCount; ++i)
		{
			mTileMaps[i]->unload();
		}

		for(int32_t i = 0; i < mTextureCount; ++i)
		{
			mTextures[i]->unload();
		}

		return STATUS_OK;
	}

	const char* GraphicsService::getPath()
	{
		Pegas_log_debug("GraphicsService::getPath");

		return NULL;
	}

	const int32_t& GraphicsService::getHeight()
	{
		Pegas_log_debug("GraphicsService::getHeight");

		return mHeight;
	}

	const int32_t& GraphicsService::getWidth()
	{
		Pegas_log_debug("GraphicsService::getWidth");

		return mWidth;
	}

	status GraphicsService::start()
	{
		Pegas_log_debug("GraphicsService::start");

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
			errorReport("eglGetDisplay");
			stop();

			return STATUS_KO;
		}

		if(!eglInitialize(mDisplay, NULL, NULL))
		{
			errorReport("eglInitialize");
			stop();

			return STATUS_KO;
		}

		if(!eglChooseConfig(mDisplay, attributes, &config, 1, &numConfig))
		{
			errorReport("eglChooseConfig");
			stop();

			return STATUS_KO;
		}

		if(!eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format))
		{
			errorReport("eglGetConfigAttrib");
			stop();

			return STATUS_KO;
		}

		ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0, format);

		mSurface = eglCreateWindowSurface(mDisplay, config, mApplication->window, NULL);
		if(mSurface == EGL_NO_SURFACE)
		{
			errorReport("eglCreateWindowSurface");
			stop();

			return STATUS_KO;
		}

		mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, NULL);
		if(mContext == EGL_NO_CONTEXT)
		{
			errorReport("eglCreateContext");
			stop();

			return STATUS_KO;
		}

		EGLBoolean makeCurrent = eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
		EGLBoolean querySurfaceWidth = eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth);
		EGLBoolean querySurfaceHeight = eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight);
		if(!makeCurrent || !querySurfaceWidth || !querySurfaceHeight
				|| (mWidth <= 0) ||  (mHeight <= 0))
		{
			errorReport("eglMakeCurrent or eglQuerySurface");
			stop();

			return STATUS_KO;
		}

		glViewport(0, 0, mWidth, mHeight);

		if(loadResources() != STATUS_OK)
		{
			errorReport("loadResources");
			stop();

			return STATUS_KO;
		}

		setup();

		return STATUS_OK;
	}

	void GraphicsService::stop()
	{
		Pegas_log_debug("GraphicsService::stop");

		unloadResources();

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

	status GraphicsService::update()
	{
		Pegas_log_debug("GraphicsService::update");

		float timeStep = mTimer->elapsed();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int32_t i = 0; i < mTileMapCount; ++i)
		{
			mTileMaps[i]->draw();
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for(int32_t i = 0; i < mSpriteCount; ++i)
		{
			mSprites[i]->draw(timeStep);
		}
		glDisable(GL_BLEND);

		if(!eglSwapBuffers(mDisplay, mSurface))
		{
			errorReport("eglSwapBuffers");
			stop();

			return STATUS_KO;
		}

		return STATUS_OK;
	}



	void GraphicsService::errorReport(const char* message)
	{
		Pegas_log_debug("GraphicsService::errorReport [message: %s]", message);

		Log::error("Error on calling function: %s", message);
		EGLint code = eglGetError();

		switch(code)
		{
		case EGL_NOT_INITIALIZED:
			Log::error("Error code: EGL_NOT_INITIALIZED");
			break;
		case EGL_BAD_ACCESS:
			Log::error("Error code: EGL_BAD_ACCESS");
			break;
		case EGL_BAD_ALLOC:
			Log::error("Error code: EGL_BAD_ALLOC");
			break;
		case EGL_BAD_ATTRIBUTE:
			Log::error("Error code: EGL_BAD_ATTRIBUTE");
			break;
		case EGL_BAD_CONFIG:
			Log::error("Error code: EGL_BAD_CONFIG");
			break;
		case EGL_BAD_CONTEXT:
			Log::error("Error code: EGL_BAD_CONTEXT");
			break;
		case EGL_BAD_CURRENT_SURFACE:
			Log::error("Error code: EGL_BAD_CURRENT_SURFACE");
			break;
		case EGL_BAD_DISPLAY:
			Log::error("Error code: EGL_BAD_DISPLAY");
			break;
		case EGL_BAD_MATCH:
			Log::error("Error code: EGL_BAD_MATCH");
			break;
		case EGL_BAD_NATIVE_PIXMAP:
			Log::error("Error code: EGL_BAD_NATIVE_PIXMAP");
			break;
		case EGL_BAD_NATIVE_WINDOW:
			Log::error("Error code: EGL_BAD_NATIVE_WINDOW");
			break;
		case EGL_BAD_PARAMETER:
			Log::error("Error code: EGL_BAD_PARAMETER");
			break;
		case EGL_BAD_SURFACE:
			Log::error("Error code: EGL_BAD_SURFACE");
			break;
		case EGL_CONTEXT_LOST:
			Log::error("Error code: EGL_CONTEXT_LOST");
			break;
		default:
			break;
		}
	}
}
