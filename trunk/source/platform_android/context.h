/*
 * Context.h
 *
 *  Created on: 26.07.2014
 *      Author: DNS
 */

#pragma once

#include "packt_types.h"

namespace packt
{
	class TimeService;
	class GraphicsService;
	class SoundService;
	class InputService;

	struct Context {
		GraphicsService* mGraphicsService;
		SoundService*	mSoundService;
		InputService* mInputService;
		TimeService* mTimeService;
	};
}



