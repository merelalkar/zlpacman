/*
 * Background.cpp
 *
 *  Created on: 16.09.2014
 *      Author: borisov.v
 */
#include "Background.h"
#include "Log.h"

namespace dbs
{
	Background::Background(Pegas::Context* pContext)
		:mTimeService(pContext->mTimeService),
		 mGraphicsService(pContext->mGraphicsService),
		 mLocation(), mAnimSpeed(8.0f)
	{
		Pegas_log_info("Background constructor [pContext: %X]", pContext);
		Pegas_log_debug("pContext->mTimeService = %X", pContext->mTimeService);
		Pegas_log_debug("pContext->mGraphicsService = %X", pContext->mGraphicsService);

		Pegas::GraphicsTexture* texture = mGraphicsService->registerTexture("tilemap.png");
		Pegas_log_debug("mGraphicsService->registerTexture [result: %X]", texture);

		mTileMap = mGraphicsService->registerTileMap("tilemap.tmx", texture, &mLocation);
		Pegas_log_debug("mGraphicsService->registerTileMap [result: %X]", mTileMap);
	}

	void Background::spawn()
	{
		Pegas_log_info("Background::spawn");
	}

	void Background::update()
	{
		Pegas_log_info_loop("Background::update");

		const float SCROLL_PER_SEC = -64.0f;
		float lScrolling = mTimeService->elapsed() * SCROLL_PER_SEC;
		Pegas_log_debug_loop("lScrolling = %f", lScrolling);

		mLocation.translate(0.0f, lScrolling);
	}
}




