/*
 * SoundService.cpp
 *
 *  Created on: 18.09.2014
 *      Author: borisov.v
 */

#include "SoundService.h"
#include "Log.h"

namespace Pegas
{
	SoundService::SoundService(android_app* app)
		: mApplication(app), mEngineObj(NULL),
		  mEngine(NULL), mOutputMixObj(NULL)
	{

	}

	status SoundService::start()
	{
		Log::info("Starting SoundService.");

		SLresult lRes;
		const SLuint32 lEngineMixIIDCount = 1;
		const SLInterfaceID lEngineMixIIDs[]= { SL_IID_ENGINE };
		const SLboolean lEngineMixReqs[] = { SL_BOOLEAN_TRUE };
		const SLuint32 lOutputMixIIDCount = 0;
		const SLInterfaceID lOutputMixIIDs[] = {};
		const SLboolean lOutputMixReqs[] = {};

		lRes = slCreateEngine(&mEngineObj, 0, NULL, lEngineMixIIDCount, lEngineMixIIDs, lEngineMixReqs);
		if (lRes != SL_RESULT_SUCCESS) goto ERROR;

		lRes = (*mEngineObj)->Realize(mEngineObj, SL_BOOLEAN_FALSE);
		if (lRes != SL_RESULT_SUCCESS) goto ERROR;

		lRes = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE, &mEngine);
		if (lRes != SL_RESULT_SUCCESS) goto ERROR;

		lRes = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj,	lOutputMixIIDCount,	lOutputMixIIDs, lOutputMixReqs);
		lRes = (*mOutputMixObj)->Realize(mOutputMixObj, SL_BOOLEAN_FALSE);

		return STATUS_OK;

		ERROR:

		Log::error("Error while starting SoundService.");
		stop();

		return STATUS_KO;
	}

	void SoundService::stop()
	{
		if (mOutputMixObj != NULL)
		{
			(*mOutputMixObj)->Destroy(mOutputMixObj);
			mOutputMixObj = NULL;
		}

		if (mEngineObj != NULL)
		{
			(*mEngineObj)->Destroy(mEngineObj);
			mEngineObj = NULL;
			mEngine = NULL;
		}
	}
}



