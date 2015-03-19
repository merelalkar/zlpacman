/*
 * Sound.h
 *
 *  Created on: 22.09.2014
 *      Author: borisov.v
 */

#pragma once

#include "packt_types.h"
#include "context.h"
#include "resource.h"

namespace packt
{
	class SoundService;

	class Sound
	{
	public:
		Sound(android_app* pApplication, const char* pPath);
		const char* getPath();

		status load();
		status unload();

	private:
		friend class SoundService;

	private:
		Resource mResource;
		uint8_t* mBuffer;
		off_t mLength;
	};
}



