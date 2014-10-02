/*
 * Sensor.cpp
 *
 *  Created on: 30.09.2014
 *      Author: borisov.v
 */
#include "sensor.h"
#include "event_loop.h"
#include "log.h"

namespace Pegas
{
	Sensor::Sensor(EventLoop& pEventLoop, int32_t pSensorType)
		:mEventLoop(pEventLoop), mSensor(NULL), mSensorType(pSensorType)
	{

	}

	status Sensor::toggle()
	{
		return (mSensor != NULL) ? disable() : enable();
	}

	status Sensor::enable()
	{
		if (mEventLoop.mEnabled)
		{
			mSensor = ASensorManager_getDefaultSensor(mEventLoop.mSensorManager, mSensorType);
			if (mSensor != NULL)
			{
				if (ASensorEventQueue_enableSensor(mEventLoop.mSensorEventQueue, mSensor) < 0)
				{
					goto ERROR;
				}

				int32_t lMinDelay = ASensor_getMinDelay(mSensor);
				if (ASensorEventQueue_setEventRate(mEventLoop.mSensorEventQueue, mSensor, lMinDelay) < 0)
				{
					goto ERROR;
				}

			} else
			{
				Pegas_log_error("No sensor type %d", mSensorType);
			}
		}

		return STATUS_OK;

		ERROR:

		Pegas_log_error("Error while activating sensor.");
		disable();

		return STATUS_KO;
	}

	status Sensor::disable()
	{
		if ((mEventLoop.mEnabled) && (mSensor != NULL))
		{
			if (ASensorEventQueue_disableSensor(mEventLoop.mSensorEventQueue, mSensor) < 0)
			{
				goto ERROR;
			}
			mSensor = NULL;
		}

		return STATUS_OK;

		ERROR:

		Pegas_log_error("Error while deactivating sensor.");

		return STATUS_KO;
	}
}



