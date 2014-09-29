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
				Pegas_log_info("Ship constructor [context: %X]", context);
				Pegas_log_debug("context->mGraphicsService = %X", context->mGraphicsService);
				Pegas_log_debug("context->mInputService = %X", context->mInputService);
				Pegas_log_debug("context->mTimeService = %X", context->mTimeService);


				Pegas::GraphicsTexture* texture = mGraphicsService->registerTexture("ship.png");
				Pegas_log_debug("texture = %X", texture);

				mSprite = mGraphicsService->registerSprite(texture, 64, 64);
				Pegas_log_debug("mSprite = %X", mSprite);
				if(mSprite)
				{
					mInputService->setRefPoint(mSprite->getLocation());
				}
			}

			void Ship::spawn()
			{
				Pegas_log_info("Ship::spawn");

				const int32_t FRAME_1 = 0;
				const int32_t FRAME_NB = 8;
				mSprite->setAnimation(FRAME_1, FRAME_NB, mAnimSpeed, true);

				int32_t width = mGraphicsService->getWidth();
				int32_t height = mGraphicsService->getHeight();
				Pegas_log_debug("width = %d, height = %d", width, height);

				float posX = width * 1.0f / 2.0f;
				float posY = height * 1.0f / 2.0f;
				Pegas_log_debug("posX = %f, posY = %f", posX, posY);

				mSprite->getLocation()->setPosition(posX, posY);
			}

			void Ship::update()
			{
				Pegas_log_info_loop("Ship::update");

				if(mSprite)
				{
					const float SPEED_PERSEC = 400.0f;
					float speed = SPEED_PERSEC * mTimeService->elapsed();
					Pegas_log_debug_loop("speed = %f", speed);

					float cx = mInputService->getHorizontal() * speed;
					float cy = mInputService->getVertical() * speed;
					Pegas_log_debug_loop("cx = %f, cy = %f", cx, cy);

					mSprite->getLocation()->translate(cx, cy);
				}
			}
}



