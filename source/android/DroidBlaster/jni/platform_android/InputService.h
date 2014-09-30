/*
 * InputService.h
 *
 *  Created on: 23.09.2014
 *      Author: borisov.v
 */

#ifndef INPUTSERVICE_H_
#define INPUTSERVICE_H_

#include "Types.h"
#include "InputHandler.h"
#include "Sensor.h"

namespace Pegas
{
	class InputService: public InputHandler
	{
	public:
		InputService(android_app* pApplication, Sensor* pAccelerometer, const int32_t& pWidth, const int32_t& pHeight);

		float getHorizontal();
		float getVertical();
		void setRefPoint(Location* pTouchReference);

		status start();
		status update();
		void stop();

	public:
		virtual bool onTouchEvent(AInputEvent* pEvent);
		virtual bool onKeyboardEvent(AInputEvent* pEvent);
		virtual bool onTrackballEvent(AInputEvent* pEvent);
		virtual bool onAccelerometerEvent(ASensorEvent* pEvent);

	private:
		android_app* mApplication;
		float mHorizontal;
		float mVertical;

		Location* mRefPoint;
		const int32_t& mWidth;
		const int32_t& mHeight;

		bool mMenuKey;

		Sensor* mAccelerometer;
	};
}

#endif /* INPUTSERVICE_H_ */
