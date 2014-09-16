/*
 * GraphicsService.h
 *
 *  Created on: 28.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_GRAPHICSSERVICE_H_
#define PEGAS_GRAPHICSSERVICE_H_

#include "GraphicsSprite.h"
#include "GraphicsTexture.h"
#include "GraphicsTileMap.h"
#include "TimeService.h"
#include "Types.h"

#include <android_native_app_glue.h>
#include <EGL/egl.h>

namespace Pegas
{
	class GraphicsService
	{
	public:
		GraphicsService(android_app* app, TimeService* timer);
		~GraphicsService();

		const char* getPath();
		const int32_t& getHeight();
		const int32_t& getWidth();

		status start();
		void stop();
		status update();

		GraphicsTexture* registerTexture(const char* pPath);
		GraphicsSprite* registerSprite(GraphicsTexture* texture, int32_t width = 0, int32_t height = 0);
		GraphicsTileMap* registerTileMap(const char* pPath, GraphicsTexture* pTexture, Location* pLocation);

	protected:
		status loadResources();
		status unloadResources();
		void setup();

	private:
		void errorReport(const char* message);

		android_app* mApplication;
		TimeService* mTimer;

		int32_t mWidth;
		int32_t mHeight;

		EGLDisplay mDisplay;
		EGLSurface mSurface;
		EGLContext mContext;

	private:
		GraphicsTexture* mTextures[32];
		int32_t mTextureCount;

		GraphicsSprite* mSprites[256];
		int32_t mSpriteCount;

		GraphicsTileMap* mTileMaps[8];
		int32_t mTileMapCount;
	};
}

#endif /* GRAPHICSSERVICE_H_ */
