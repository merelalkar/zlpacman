/*
 * InputHandler.h
 *
 *  Created on: 23.09.2014
 *      Author: borisov.v
 */

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include "Types.h"

#include <android_native_app_glue.h>
#include <android/input.h>
#include <android/sensor.h>

namespace Pegas
{
	class InputHandler
	{
	public:
		virtual ~InputHandler() {}
		virtual bool onTouchEvent(AInputEvent* pEvent) = 0;
		virtual bool onKeyboardEvent(AInputEvent* pEvent) = 0;
		virtual bool onTrackballEvent(AInputEvent* pEvent) = 0;
		virtual bool onAccelerometerEvent(ASensorEvent* pEvent) = 0;
	};
}


#endif /* INPUTHANDLER_H_ */
