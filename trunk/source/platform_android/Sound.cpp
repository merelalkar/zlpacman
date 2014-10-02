/*
 * Sound.cpp
 *
 *  Created on: 22.09.2014
 *      Author: borisov.v
 */

#include "sound.h"
#include "log.h"

namespace Pegas
{
	Sound::Sound(android_app* pApplication, const char* pPath)
		:mResource(pApplication, pPath), mBuffer(NULL), mLength(0)
	{

	}

	const char* Sound::getPath()
	{
		return mResource.getPath();
	}

	status Sound::load()
	{
		status lRes;

		if (mResource.open() != STATUS_OK)
		{
			return STATUS_KO;
		}

		mLength = mResource.getLength();
		mBuffer = new uint8_t[mLength];
		lRes = mResource.read(mBuffer, mLength);
		mResource.close();

		if (lRes != STATUS_OK)
		{
			Pegas_log_error("Error while reading PCM sound.");
			return STATUS_KO;
		}

		return STATUS_OK;
	}

	status Sound::unload()
	{
		delete[] mBuffer;
		mBuffer = NULL;
		mLength = 0;

		return STATUS_OK;
	}
}


