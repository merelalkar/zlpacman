/*
 * Context.h
 *
 *  Created on: 26.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_CONTEXT_H_
#define PEGAS_CONTEXT_H_

#include "Types.h"

namespace Pegas
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


#endif /* CONTEXT_H_ */
