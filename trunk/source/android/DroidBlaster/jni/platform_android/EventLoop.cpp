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
		:mApplication(pApplication), mActivityHandler(NULL), mInputHandler(NULL),
		 mEnabled(false), mQuit(false)
	{
		Pegas_log_debug("EventLoop constructor [pApplication: %X]", pApplication);

		mApplication->onAppCmd = callback_activity;
		mApplication->userData = this;
	}

	void EventLoop::run(ActivityHandler* pActivityHandler, InputHandler* pInputHandler)
	{
		Pegas_log_debug("EventLoop::run");

		int32_t lResult;
		int32_t lEvents;
		android_poll_source* lSource;

		app_dummy();

		mActivityHandler = pActivityHandler;
		mInputHandler = pInputHandler;

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
				if(mActivityHandler->onStep() != STATUS_OK)
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

			if(mActivityHandler->onActivate() != STATUS_OK)
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
			mActivityHandler->onDeactivate();
			mEnabled = false;
		}
	}

	void EventLoop::processActivityEvent(int32_t pCommand)
	{
		Pegas_log_debug("EventLoop::processActivityEvent [pCommand: %d]", pCommand);

		switch(pCommand)
		{
		case APP_CMD_CONFIG_CHANGED:
			mActivityHandler->onConfigurationChanged();
			break;
		case APP_CMD_INIT_WINDOW:
			mActivityHandler->onCreateWindow();
			break;
		case APP_CMD_DESTROY:
			mActivityHandler->onDestroy();
			break;
		case APP_CMD_GAINED_FOCUS:
			activate();
			mActivityHandler->onGainFocus();
			break;
		case APP_CMD_LOST_FOCUS:
			mActivityHandler->onLostFocus();
			deactivate();
			break;
		case APP_CMD_LOW_MEMORY:
			mActivityHandler->onLowMemory();
			break;
		case APP_CMD_PAUSE:
			mActivityHandler->onPause();
			deactivate();
			break;
		case APP_CMD_RESUME:
			mActivityHandler->onResume();
			break;
		case APP_CMD_SAVE_STATE:
			mActivityHandler->onSaveState(&mApplication->savedState,
					(int32_t*)&mApplication->savedStateSize);
			break;
		case APP_CMD_START:
			mActivityHandler->onStart();
			break;
		case APP_CMD_STOP:
			mActivityHandler->onStop();
			break;
		case APP_CMD_TERM_WINDOW:
			mActivityHandler->onDestroyWindow();
			deactivate();
			break;
		default:
			break;
		}
	}

	int32_t EventLoop::processInputEvent(AInputEvent* pEvent)
	{
		int32_t lEventType = AInputEvent_getType(pEvent);
		switch (lEventType)
		{
		case AINPUT_EVENT_TYPE_MOTION:
			switch (AInputEvent_getSource(pEvent))
			{
			case AINPUT_SOURCE_TOUCHSCREEN:
				return mInputHandler->onTouchEvent(pEvent);
				break;
			}
			break;
		}

		return 0;
	}

	void EventLoop::processSensorEvent()
	{

	}

	void EventLoop::callback_activity(android_app* pApplication, int32_t pCommand)
	{
		Pegas_log_debug("EventLoop::callback_activity [pApplication: %X, pCommand: %d]", pApplication, pCommand);

		EventLoop* pInstance = (EventLoop*)pApplication->userData;
		pInstance->processActivityEvent(pCommand);
	}

	int32_t EventLoop::callback_input(android_app* pApplication, AInputEvent* pEvent)
	{
		Pegas_log_debug("EventLoop::activityCallBack [pApplication: %X, pEvent: %d]", pApplication, pEvent);

		EventLoop& lEventLoop = *(EventLoop*)pApplication->userData;

		return lEventLoop.processInputEvent(pEvent);
	}
}



