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
		 mRefPoint(NULL), mHorizontal(0.0f), mVertical(0.0f), mMenuKey(false)
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

	status InputService::update()
	{
		if(mMenuKey)
		{
			return STATUS_EXIT;
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

	bool InputService::onKeyboardEvent(AInputEvent* pEvent)
	{
		const float ORTHOGONAL_MOVE = 1.0f;
		int32_t lAction = AKeyEvent_getAction(pEvent);
		int32_t lKeyCode = AKeyEvent_getKeyCode(pEvent);

		if(lAction == AKEY_EVENT_ACTION_DOWN)
		{
			switch (lKeyCode)
			{
			case AKEYCODE_DPAD_LEFT:
				mHorizontal = -ORTHOGONAL_MOVE;
				break;
			case AKEYCODE_DPAD_RIGHT:
				mHorizontal = ORTHOGONAL_MOVE;
				break;
			case AKEYCODE_DPAD_DOWN:
				mVertical = -ORTHOGONAL_MOVE;
				break;
			case AKEYCODE_DPAD_UP:
				mVertical = ORTHOGONAL_MOVE;
				break;
			case AKEYCODE_BACK:
				return false;
			}
		} else
		{
			switch (lKeyCode)
			{
			case AKEYCODE_DPAD_LEFT:
			case AKEYCODE_DPAD_RIGHT:
				mHorizontal = 0.0f;
				break;
			case AKEYCODE_DPAD_DOWN:
			case AKEYCODE_DPAD_UP:
				mVertical = 0.0f;
				break;
			case AKEYCODE_MENU:
				mMenuKey = true;
				break;
			case AKEYCODE_BACK:
				return false;
			}
		}

		return true;
	}

	bool InputService::onTrackballEvent(AInputEvent* pEvent)
	{
		const float ORTHOGONAL_MOVE = 1.0f;
		const float DIAGONAL_MOVE = 0.707f;
		const float THRESHOLD = (1/100.0f);

		if (AMotionEvent_getAction(pEvent) == AMOTION_EVENT_ACTION_MOVE)
		{
			float lDirectionX = AMotionEvent_getX(pEvent, 0);
			float lDirectionY = AMotionEvent_getY(pEvent, 0);
			float lHorizontal, lVertical;

			if (lDirectionX < -THRESHOLD)
			{
				if (lDirectionY < -THRESHOLD)
				{
					lHorizontal = -DIAGONAL_MOVE;
					lVertical = DIAGONAL_MOVE;
				} else if (lDirectionY > THRESHOLD)
				{
					lHorizontal = -DIAGONAL_MOVE;
					lVertical = -DIAGONAL_MOVE;
				} else
				{
					lHorizontal = -ORTHOGONAL_MOVE;
					lVertical = 0.0f;
				}

			} else if (lDirectionX > THRESHOLD)
			{
				if (lDirectionY < -THRESHOLD)
				{
					lHorizontal = DIAGONAL_MOVE;
					lVertical = DIAGONAL_MOVE;
				} else if (lDirectionY > THRESHOLD)
				{
					lHorizontal = DIAGONAL_MOVE;
					lVertical = -DIAGONAL_MOVE;
				} else
				{
					lHorizontal = ORTHOGONAL_MOVE;
					lVertical = 0.0f;
				}

			} else if (lDirectionY < -THRESHOLD)
			{
				lHorizontal = 0.0f;
				lVertical = ORTHOGONAL_MOVE;
			} else if (lDirectionY > THRESHOLD)
			{
				lHorizontal = 0.0f;
				lVertical = -ORTHOGONAL_MOVE;
			}

			// Ends movement if there is a counter movement.
			if ((lHorizontal < 0.0f) && (mHorizontal > 0.0f))
			{
				mHorizontal = 0.0f;
			} else if((lHorizontal > 0.0f)&&(mHorizontal < 0.0f))
			{
				mHorizontal = 0.0f;
			} else
			{
				mHorizontal = lHorizontal;
			}

			if ((lVertical < 0.0f) && (mVertical > 0.0f))
			{
				mVertical = 0.0f;
			} else if ((lVertical > 0.0f) && (mVertical < 0.0f))
			{
				mVertical = 0.0f;
			} else
			{
				mVertical = lVertical;
			}
		} else
		{
			mHorizontal = 0.0f;
			mVertical = 0.0f;
		}

		return true;
	}
}
