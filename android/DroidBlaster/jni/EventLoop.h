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

#include <android_native_app_glue.h>

namespace Pegas
{
	class EventLoop
	{
	public:
		EventLoop(android_app* pApplication);

		void run(ActivityHandler& handler);
	protected:
		void activate();
		void deactivate();

		void processActivityEvent(int32_t pCommand);

	private:
		static void activityCallBack(android_app* pApplication, int32_t pCommand);

		ActivityHandler*	mHandler;
		android_app* 		mApplication;
		bool		 		mEnabled;
		bool		 		mQuit;

	};
}

#endif /* EVENTLOOP_H_ */
