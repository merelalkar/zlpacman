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
		mTileMap = mGraphicsService->registerTileMap("tilemap.tmx", mGraphicsService->registerTexture("tilemap.png"), &mLocation);
	}

	void Background::spawn()
	{

	}

	void Background::update()
	{
		const float SCROLL_PER_SEC = -64.0f;
		float lScrolling = mTimeService->elapsed() * SCROLL_PER_SEC;

		mLocation.translate(0.0f, lScrolling);
	}
}




