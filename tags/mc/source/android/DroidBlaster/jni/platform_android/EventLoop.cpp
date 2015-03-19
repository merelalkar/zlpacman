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
		 mSensorPollSource(), mSensorManager(NULL), mSensorEventQueue(NULL),
		 mEnabled(false), mQuit(false)
	{
		Pegas_log_info("EventLoop constructor [pApplication: %X]", pApplication);

		mApplication->onAppCmd = callback_activity;
		mApplication->onInputEvent = callback_input;
		mApplication->userData = this;
	}

	void EventLoop::run(ActivityHandler* pActivityHandler, InputHandler* pInputHandler)
	{
		Pegas_log_info_loop("EventLoop::run [pActivityHandler: %X, pInputHandler: %X]", pActivityHandler, pInputHandler);

		int32_t lResult;
		int32_t lEvents;
		android_poll_source* lSource;

		app_dummy();

		mActivityHandler = pActivityHandler;
		mInputHandler = pInputHandler;

		Pegas_log_info("Starting event loop");
		while (true)
		{
			while ((lResult = ALooper_pollAll(mEnabled ? 0 : -1,
					NULL, &lEvents, (void**)&lSource)) >= 0)
			{
				if (lSource != NULL)
				{
					Pegas_log_info_loop("Processing an event");
					lSource->process(mApplication, lSource);
				}//if (lSource != NULL)

				if (mApplication->destroyRequested)
				{
					Pegas_log_info("Exiting event loop");
					return;
				}//if (mApplication->destroyRequested)
			}//while ((lResult = ALooper_pollAll(-1, NULL, &lEvents, (void**)&lSource)) >= 0)

			if(mEnabled && !mQuit)
			{
				if(mActivityHandler->onStep() != STATUS_OK)
				{
					Pegas_log_info("Application finish");
					mQuit = true;
					ANativeActivity_finish(mApplication->activity);
				}
			}
		}//while (true)
	}

	void EventLoop::activate()
	{
		Pegas_log_info("EventLoop::activate");

		if((!mEnabled) && (mApplication->window != NULL))
		{
			mSensorPollSource.id = LOOPER_ID_USER;
			mSensorPollSource.app = mApplication;
			mSensorPollSource.process = callback_sensor;
			mSensorManager = ASensorManager_getInstance();
			if (mSensorManager != NULL)
			{
				mSensorEventQueue = ASensorManager_createEventQueue(mSensorManager,
						mApplication->looper, LOOPER_ID_USER, NULL, &mSensorPollSource);
				if (mSensorEventQueue == NULL)
				{
					Pegas_log_warning("EventLoop::activate:");
					Pegas_log_warning("ASensorManager_createEventQueue == NULL");

					goto ERROR;
				}
			}
			mQuit = false;
			mEnabled = true;

			if(mActivityHandler->onActivate() != STATUS_OK)
			{
				Pegas_log_warning("EventLoop::activate:");
				Pegas_log_warning("mActivityHandler->onActivate() != STATUS_OK");

				goto ERROR;

			}
		}
		return;

		ERROR:

		Pegas_log_info("Application finish");
		mQuit = true;
		deactivate();
		ANativeActivity_finish(mApplication->activity);
	}

	void EventLoop::deactivate()
	{
		Pegas_log_info("EventLoop::deactivate");

		if(mEnabled)
		{
			mActivityHandler->onDeactivate();
			mEnabled = false;

			if (mSensorEventQueue != NULL)
			{
				ASensorManager_destroyEventQueue(mSensorManager, mSensorEventQueue);
				mSensorEventQueue = NULL;
			}
			mSensorManager = NULL;
		}
	}

	void EventLoop::processActivityEvent(int32_t pCommand)
	{
		Pegas_log_info_loop("EventLoop::processActivityEvent [pCommand: %d]", pCommand);

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
		Pegas_log_info_loop("EventLoop::processInputEvent [pEvent: %d]", pEvent);

		int32_t lEventType = AInputEvent_getType(pEvent);
		Pegas_log_debug_loop("lEventType = %d [%X]", lEventType, lEventType);
		int32_t lEventSource = AInputEvent_getSource(pEvent);
		Pegas_log_debug_loop("lEventSource = %d [%X]", lEventSource, lEventSource);

		switch (lEventType)
		{
		case AINPUT_EVENT_TYPE_MOTION:
			switch (lEventSource)
			{
			case AINPUT_SOURCE_TOUCHSCREEN:
				return mInputHandler->onTouchEvent(pEvent);
			case AINPUT_SOURCE_TRACKBALL:
				return mInputHandler->onTrackballEvent(pEvent);
			}
			break;

		case AINPUT_EVENT_TYPE_KEY:
			return mInputHandler->onKeyboardEvent(pEvent);
		}


		return 0;
	}

	void EventLoop::processSensorEvent()
	{
		Pegas_log_info_loop("EventLoop::processSensorEvent");

		ASensorEvent lEvent;
		while (ASensorEventQueue_getEvents(mSensorEventQueue, &lEvent, 1) > 0)
		{
			switch (lEvent.type)
			{
			case ASENSOR_TYPE_ACCELEROMETER:
				mInputHandler->onAccelerometerEvent(&lEvent);
				break;
			}
		}
	}

	void EventLoop::callback_activity(android_app* pApplication, int32_t pCommand)
	{
		Pegas_log_info_loop("EventLoop::callback_activity [pApplication: %X, pCommand: %d]", pApplication, pCommand);

		EventLoop* pInstance = (EventLoop*)pApplication->userData;
		pInstance->processActivityEvent(pCommand);
	}

	int32_t EventLoop::callback_input(android_app* pApplication, AInputEvent* pEvent)
	{
		Pegas_log_info_loop("EventLoop::callback_input [pApplication: %X, pEvent: %d]", pApplication, pEvent);

		EventLoop& lEventLoop = *(EventLoop*)pApplication->userData;

		return lEventLoop.processInputEvent(pEvent);
	}

	void EventLoop::callback_sensor(android_app* pApplication, android_poll_source* pSource)
	{
		EventLoop& lEventLoop = *(EventLoop*)pApplication->userData;
		lEventLoop.processSensorEvent();
	}
}



