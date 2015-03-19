/*
 * EventLoop.h
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#pragma once

#include "packt_types.h"
#include "activity_handler.h"
#include "input_handler.h"

#include <android_native_app_glue.h>
#include <android/sensor.h>

namespace packt
{
	class EventLoop
	{
	public:
		EventLoop(android_app* pApplication);

		void run(ActivityHandler* pActivityHandler, InputHandler* pInputHandler);
	protected:
		void activate();
		void deactivate();

		void processActivityEvent(int32_t pCommand);
		int32_t processInputEvent(AInputEvent* pEvent);
		void processSensorEvent();

	private:
		friend class Sensor;

		static void callback_activity(android_app* pApplication, int32_t pCommand);
		static int32_t callback_input(android_app* pApplication, AInputEvent* pEvent);
		static void callback_sensor(android_app* pApplication, android_poll_source* pSource);

	private:

		ActivityHandler*	mActivityHandler;
		InputHandler* 		mInputHandler;
		android_app* 		mApplication;
		bool		 		mEnabled;
		bool		 		mQuit;

		ASensorManager* mSensorManager;
		ASensorEventQueue* mSensorEventQueue;
		android_poll_source mSensorPollSource;
	};
}


