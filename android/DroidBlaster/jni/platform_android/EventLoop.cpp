/*
 * EventLoop.cpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */
#include "EventLoop.h"
#include "Log.h"

namespace Pegas
{
	EventLoop::EventLoop(android_app* pApplication)
		:mApplication(pApplication), mHandler(NULL),
		 mEnabled(false), mQuit(false)
	{
		Pegas_log_debug("EventLoop constructor [pApplication: %X]", pApplication);

		mApplication->onAppCmd = activityCallBack;
		mApplication->userData = this;
	}

	void EventLoop::run(ActivityHandler& handler)
	{
		Pegas_log_debug("EventLoop::run");

		int32_t lResult;
		int32_t lEvents;
		android_poll_source* lSource;

		app_dummy();

		mHandler = &handler;

		Log::info("Starting event loop");
		while (true)
		{
			while ((lResult = ALooper_pollAll(mEnabled ? 0 : -1,
					NULL, &lEvents, (void**)&lSource)) >= 0)
			{
				if (lSource != NULL)
				{
					Log::info("Processing an event");
					lSource->process(mApplication, lSource);
				}//if (lSource != NULL)

				if (mApplication->destroyRequested)
				{
					Log::info("Exiting event loop");
					return;
				}//if (mApplication->destroyRequested)
			}//while ((lResult = ALooper_pollAll(-1, NULL, &lEvents, (void**)&lSource)) >= 0)

			if(mEnabled && !mQuit)
			{
				if(mHandler->onStep() != STATUS_OK)
				{
					mQuit = true;
					ANativeActivity_finish(mApplication->activity);
				}
			}
		}//while (true)
	}

	void EventLoop::activate()
	{
		Pegas_log_debug("EventLoop::activate");

		if((!mEnabled) && (mApplication->window != NULL)){
			mQuit = false;
			mEnabled = true;

			if(mHandler->onActivate() != STATUS_OK)
			{
				mQuit = true;
				ANativeActivity_finish(mApplication->activity);
			}
		}
	}

	void EventLoop::deactivate()
	{
		Pegas_log_debug("EventLoop::deactivate");

		if(mEnabled)
		{
			mHandler->onDeactivate();
			mEnabled = false;
		}
	}

	void EventLoop::processActivityEvent(int32_t pCommand)
	{
		Pegas_log_debug("EventLoop::processActivityEvent [pCommand: %d]", pCommand);

		switch(pCommand)
		{
		case APP_CMD_CONFIG_CHANGED:
			mHandler->onConfigurationChanged();
			break;
		case APP_CMD_INIT_WINDOW:
			mHandler->onCreateWindow();
			break;
		case APP_CMD_DESTROY:
			mHandler->onDestroy();
			break;
		case APP_CMD_GAINED_FOCUS:
			activate();
			mHandler->onGainFocus();
			break;
		case APP_CMD_LOST_FOCUS:
			mHandler->onLostFocus();
			deactivate();
			break;
		case APP_CMD_LOW_MEMORY:
			mHandler->onLowMemory();
			break;
		case APP_CMD_PAUSE:
			mHandler->onPause();
			deactivate();
			break;
		case APP_CMD_RESUME:
			mHandler->onResume();
			break;
		case APP_CMD_SAVE_STATE:
			mHandler->onSaveState(&mApplication->savedState,
					(int32_t*)&mApplication->savedStateSize);
			break;
		case APP_CMD_START:
			mHandler->onStart();
			break;
		case APP_CMD_STOP:
			mHandler->onStop();
			break;
		case APP_CMD_TERM_WINDOW:
			mHandler->onDestroyWindow();
			deactivate();
			break;
		default:
			break;
		}
	}

	void EventLoop::activityCallBack(android_app* pApplication, int32_t pCommand)
	{
		Pegas_log_debug("EventLoop::activityCallBack [pApplication: %X, pCommand: %d]", pApplication, pCommand);

		EventLoop* pInstance = (EventLoop*)pApplication->userData;
		pInstance->processActivityEvent(pCommand);
	}
}



