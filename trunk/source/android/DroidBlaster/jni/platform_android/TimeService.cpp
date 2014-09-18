/*
 * TimeService.cpp
 *
 *  Created on: 26.07.2014
 *      Author: DNS
 */
#include "TimeService.h"
#include "Log.h"

namespace Pegas
{

	TimeService::TimeService()
		:mElapsed(0.0f), mLastTime(0.0)
	{

	}

	void TimeService::reset()
	{
		Log::info("TimeServise::reset()");
		mElapsed = 0.0f;
		mLastTime = now();
	}

	void TimeService::update()
	{
		double currentTime = now();
		mElapsed = currentTime - mLastTime;
		mLastTime = currentTime;
	}

	double TimeService::now()
	{
		timespec timeValue;
		clock_gettime(CLOCK_MONOTONIC, &timeValue);

		return (timeValue.tv_sec + (timeValue.tv_nsec * 1.0e-9));
	}

	float TimeService::elapsed()
	{
		return mElapsed;
	}
}
