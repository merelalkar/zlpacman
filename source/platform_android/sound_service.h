/*
 * SoundService.h
 *
 *  Created on: 18.09.2014
 *      Author: borisov.v
 */

#ifndef SOUNDSERVICE_H_
#define SOUNDSERVICE_H_

#include "packt_types.h"
#include "sound.h"

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
		~SoundService();

		status start();
		void stop();

		status playBackgroundMusic(const char* pPath);
		void stopBackgroundMusic();

		Sound* registerSound(const char* pPath);
		status playSound(Sound* pSound);

	private:
		status startSoundPlayer();
		static void callback_sound(SLBufferQueueItf pObject, void* pContext);

	private:
		android_app* mApplication;
		SLObjectItf mEngineObj;
		SLEngineItf mEngine;
		SLObjectItf mOutputMixObj;

		//music
		SLObjectItf mBGMPlayerObj;
		SLPlayItf mBGMPlayer;
		SLSeekItf mBGMPlayerSeek;

		//sounds
		SLObjectItf mPlayerObj;
		SLPlayItf mPlayer;
		SLBufferQueueItf mPlayerQueue;
		Sound* mSounds[32];
		int32_t mSoundCount;
	};
}

#endif /* SOUNDSERVICE_H_ */
