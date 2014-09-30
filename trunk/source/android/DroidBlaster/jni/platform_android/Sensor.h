/*
 * Sensor.h
 *
 *  Created on: 30.09.2014
 *      Author: borisov.v
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "Types.h"

#include <android/sensor.h>

namespace Pegas
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

#endif /* SENSOR_H_ */
