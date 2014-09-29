/*
 * DroidBlaster.cpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */
#include "DroidBlaster.h"
#include "Types.h"
#include "Log.h"
#include "TimeService.h"

#include <unistd.h>
#include <math.h>

using namespace Pegas;

namespace dbs
{
	DroidBlaster::DroidBlaster(Pegas::Context& context, android_app* app)
		:mGraphicsService(context.mGraphicsService),
		 mSoundService(context.mSoundService),
		 mInputService(context.mInputService),
		 mTimeService(context.mTimeService), mBackground(NULL),
		 mStartSound(mSoundService->registerSound("start.pcm"))
	{
		Pegas_log_info("DroidBlaster constructor");

		mBackground = new Background(&context);
		mShip = new Ship(&context);
	}

	DroidBlaster::~DroidBlaster()
	{
		Pegas_log_info("DroidBlaster destructor");

		delete mShip;
		delete mBackground;
	}

	status DroidBlaster::onActivate()
	{
		Pegas_log_info("DroidBlaster::onActivate");


		if(mGraphicsService->start() != STATUS_OK)
		{
			Pegas::Log::warn("mGraphicsService->start() != STATUS_OK");
			return STATUS_KO;
		}

		if(mInputService->start() != STATUS_OK)
		{
			Pegas::Log::warn("mInputService->update() != STATUS_OK");
			return STATUS_KO;
		}

		if(mSoundService->start() != STATUS_OK)
		{
			Pegas::Log::warn("mSoundService->start() != STATUS_OK");
			return STATUS_KO;
		}

		mSoundService->playBackgroundMusic("bgm.mp3");
		mSoundService->playSound(mStartSound);

		mBackground->spawn();
		mShip->spawn();

		mTimeService->reset();

		return STATUS_OK;
	}

	void DroidBlaster::onDeactivate()
	{
		Pegas_log_info("DroidBlaster::onDeactivate");

		mSoundService->stop();
		mInputService->stop();
		mGraphicsService->stop();
	}

	status DroidBlaster::onStep()
	{
		Pegas_log_info_loop("DroidBlaster::onStep");

		mTimeService->update();
		mBackground->update();
		mShip->update();

		if(mInputService->update() != STATUS_OK)
		{
			Pegas::Log::warn("mInputService->update() != STATUS_OK");
			return STATUS_KO;
		}

		if(mGraphicsService->update() != STATUS_OK)
		{
			Pegas::Log::warn("mGraphicsService->update() != STATUS_OK");
			return STATUS_KO;
		}

		return STATUS_OK;
	}

	void DroidBlaster::onStart()
	{
		Pegas_log_info("DroidBlaster::onStart");
	}

	void DroidBlaster::onResume()
	{
		Pegas_log_info("DroidBlaster::onResume");
	}

	void DroidBlaster::onPause()
	{
		Pegas_log_info("DroidBlaster::onPause");
	}

	void DroidBlaster::onStop()
	{
		Pegas_log_info("DroidBlaster::onStop");
	}

	void DroidBlaster::onDestroy()
	{
		Pegas_log_info("DroidBlaster::onDestroy");
	}

	void DroidBlaster::onSaveState(void** pData, int32_t* pSize)
	{
		Pegas_log_info("DroidBlaster::onSaveState");
	}

	void DroidBlaster::onConfigurationChanged()
	{
		Pegas_log_info("DroidBlaster::onConfigurationChanged");
	}

	void DroidBlaster::onLowMemory()
	{
		Pegas_log_info("DroidBlaster::onLowMemory");
	}

	void DroidBlaster::onCreateWindow()
	{
		Pegas_log_info("DroidBlaster::onCreateWindow");
	}

	void DroidBlaster::onDestroyWindow()
	{
		Pegas_log_info("DroidBlaster::onDestroyWindow");
	}

	void DroidBlaster::onGainFocus()
	{
		Pegas_log_info("DroidBlaster::onGainFocus");
	}

	void DroidBlaster::onLostFocus()
	{
		Pegas_log_info("DroidBlaster::onLostFocus");
	}
}





