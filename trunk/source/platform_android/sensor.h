/*
 * Sensor.h
 *
 *  Created on: 30.09.2014
 *      Author: borisov.v
 */

#pragma once

#include "packt_types.h"

#include <android/sensor.h>

namespace packt
{
	class EventLoop;

	class Sensor
	{
	public:
		Sensor(EventLoop& pEventLoop, int32_t pSensorType);

		status toggle();
		status enable();
		status disable();
	private:

		EventLoop& mEventLoop;
		const ASensor* mSensor;
		int32_t mSensorType;
	};
}


