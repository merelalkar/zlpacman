/*
 * InputService.cpp
 *
 *  Created on: 23.09.2014
 *      Author: borisov.v
 */

#include "InputService.h"
#include "Log.h"

#include <math.h>

namespace Pegas
{
	InputService::InputService(android_app* pApplication, const int32_t& pWidth, const int32_t& pHeight)
		:mApplication(pApplication), mWidth(pWidth), mHeight(pHeight),
		 mRefPoint(NULL), mHorizontal(0.0f), mVertical(0.0f)
	{

	}

	float InputService::getHorizontal()
	{
		return mHorizontal;
	}

	float InputService::getVertical()
	{
		return mVertical;
	}

	void InputService::setRefPoint(Location* pTouchReference)
	{
		mRefPoint = pTouchReference;
	}

	status InputService::start()
	{
		mHorizontal = 0.0f;
		mVertical = 0.0f;

		if ((mWidth == 0) || (mHeight == 0))
		{
			return STATUS_KO;
		}

		return STATUS_OK;
	}

	bool InputService::onTouchEvent(AInputEvent* pEvent)
	{
		const float TOUCH_MAX_RANGE = 65.0f; // In pixels.

		if (mRefPoint != NULL)
		{
			if (AMotionEvent_getAction(pEvent) == AMOTION_EVENT_ACTION_MOVE)
			{
				// Needs a conversion to proper coordinates
				// (origin at bottom/left). Only lMoveY needs it.
				float lMoveX = AMotionEvent_getX(pEvent, 0) - mRefPoint->mPosX;
				float lMoveY = mHeight - AMotionEvent_getY(pEvent, 0) - mRefPoint->mPosY;
				float lMoveRange = sqrt((lMoveX * lMoveX) + (lMoveY * lMoveY));

				if (lMoveRange > TOUCH_MAX_RANGE)
				{
					float lCropFactor = TOUCH_MAX_RANGE / lMoveRange;
					lMoveX *= lCropFactor;
					lMoveY *= lCropFactor;
				}

				mHorizontal = lMoveX / TOUCH_MAX_RANGE;
				mVertical = lMoveY / TOUCH_MAX_RANGE;
			} else
			{
				mHorizontal = 0.0f;
				mVertical = 0.0f;
			}
		}

		return true;
	}
}
