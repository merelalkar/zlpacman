/*
 * SoundService.h
 *
 *  Created on: 18.09.2014
 *      Author: borisov.v
 */

#ifndef SOUNDSERVICE_H_
#define SOUNDSERVICE_H_

#include "Types.h"

#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

namespace Pegas
{
	class SoundService
	{
	public:
		SoundService(android_app* app);

		status start();
		void stop();

	private:
		android_app* mApplication;
		SLObjectItf mEngineObj;
		SLEngineItf mEngine;
		SLObjectItf mOutputMixObj;
	};
}

#endif /* SOUNDSERVICE_H_ */
