/*
 * EventLoop.h
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_EVENTLOOP_H_
#define PEGAS_EVENTLOOP_H_

#include "Types.h"
#include "ActivityHandler.h"
#include "InputHandler.h"

#include <android_native_app_glue.h>

namespace Pegas
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
		static void callback_activity(android_app* pApplication, int32_t pCommand);
		static int32_t callback_input(android_app* pApplication, AInputEvent* pEvent);

		ActivityHandler*	mActivityHandler;
		InputHandler* 		mInputHandler;
		android_app* 		mApplication;
		bool		 		mEnabled;
		bool		 		mQuit;

	};
}

#endif /* EVENTLOOP_H_ */
