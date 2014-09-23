/*
 * Ship.cpp
 *
 *  Created on: 08.09.2014
 *      Author: DNS
 */
#include "Ship.h"
#include "Log.h"

namespace dbs
{

			Ship::Ship(Pegas::Context* context)
				:mGraphicsService(context->mGraphicsService),
				 mInputService(context->mInputService),
				 mTimeService(context->mTimeService),
				 mAnimSpeed(8.0f)
			{
				Pegas_log_debug("Ship constructor [context: %X]", context);


				Pegas::GraphicsTexture* texture = mGraphicsService->registerTexture("ship.png");

				mSprite = mGraphicsService->registerSprite(texture);
				if(mSprite)
				{
					mInputService->setRefPoint(mSprite->getLocation());
				}
			}

			void Ship::spawn()
			{
				Pegas_log_debug("Ship::spawn");

				const int32_t FRAME_1 = 0;
				const int32_t FRAME_NB = 8;

				mSprite->setAnimation(FRAME_1, FRAME_NB, mAnimSpeed, true);
				mSprite->getLocation()->setPosition(mGraphicsService->getWidth() * 1.0f / 2.0f, mGraphicsService->getHeight() * 1.0f / 2.0f);
			}

			void Ship::update()
			{
				if(mSprite)
				{
					const float SPEED_PERSEC = 400.0f;
					float speed = SPEED_PERSEC * mTimeService->elapsed();
					float cx = mInputService->getHorizontal() * speed;
					float cy = mInputService->getVertical() * speed;

					mSprite->getLocation()->translate(cx, cy);
				}
			}
}



