/*
 * Ship.h
 *
 *  Created on: 08.09.2014
 *      Author: DNS
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "../engine/Context.h"
#include "../engine/GraphicsService.h"
#include "../engine/GraphicsSprite.h"
#include "../engine/Types.h"


namespace dbs
{
		class Ship
		{
		public:
			Ship(Pegas::Context* context);

			void spawn();

		private:
			Pegas::GraphicsService* mGraphicsService;
			Pegas::GraphicsSprite* mSprite;
			float mAnimSpeed;
		};
}


#endif /* SHIP_H_ */
