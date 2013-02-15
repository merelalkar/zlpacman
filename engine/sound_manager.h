#ifndef PEGAS_SOUND_MANAGER_H
#define PEGAS_SOUND_MANAGER_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{

	class SoundManager: public Singleton<SoundManager>
	{
	public:
		SoundManager(): Singleton(*this) {};
		virtual ~SoundManager() {};

		virtual void playSound(RESOURCEID soundID) = 0;
	private:
		
		SoundManager(const SoundManager& src);
		SoundManager& operator=(const SoundManager& src);
	};
}

#endif PEGAS_SOUND_MANAGER_H