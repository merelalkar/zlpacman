/*
 * TimeService.h
 *
 *  Created on: 26.07.2014
 *      Author: DNS
 */

#pragma once

#include "packt_types.h"

#include <time.h>

namespace packt
{
	class TimeService
	{
	public:
		TimeService();

		void reset();
		void update();

		double now();
		float elapsed();

	private:
		float mElapsed;
		double mLastTime;
	};
}


