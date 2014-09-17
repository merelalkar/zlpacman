/*
 * Background.h
 *
 *  Created on: 16.09.2014
 *      Author: borisov.v
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "../engine/Context.h"
#include "../engine/GraphicsService.h"
#include "../engine/GraphicsTileMap.h"
#include "../engine/Types.h"

namespace dbs
{
	class Background
	{
	public:
		Background(Pegas::Context* pContext);

		void spawn();
		void update();

	private:
		Pegas::TimeService* mTimeService;
		Pegas::GraphicsService* mGraphicsService;
		Pegas::GraphicsTileMap* mTileMap;
		Pegas::Location mLocation;
		float mAnimSpeed;
	};
}

#endif /* BACKGROUND_H_ */
