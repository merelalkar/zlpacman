/*
 * Sound.h
 *
 *  Created on: 22.09.2014
 *      Author: borisov.v
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "packt_types.h"
#include "context.h"
#include "resource.h"

namespace Pegas
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


#endif /* SOUND_H_ */
