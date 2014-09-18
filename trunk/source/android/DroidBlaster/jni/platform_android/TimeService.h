/*
 * TimeService.h
 *
 *  Created on: 26.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_TIMESERVICE_H_
#define PEGAS_TIMESERVICE_H_

#include "Types.h"

#include <time.h>

namespace Pegas
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

#endif /* TIMESERVICE_H_ */
