/*
 * Ship.h
 *
 *  Created on: 08.09.2014
 *      Author: DNS
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "Context.h"
#include "InputService.h"
#include "TimeService.h"
#include "GraphicsService.h"
#include "GraphicsSprite.h"
#include "Types.h"


namespace dbs
{
		class Ship
		{
		public:
			Ship(Pegas::Context* context);

			void spawn();
			void update();

		private:
			Pegas::TimeService* 	mTimeService;
			Pegas::InputService*	mInputService;
			Pegas::GraphicsService* mGraphicsService;
			Pegas::GraphicsSprite* mSprite;
			float mAnimSpeed;
		};
}


#endif /* SHIP_H_ */
