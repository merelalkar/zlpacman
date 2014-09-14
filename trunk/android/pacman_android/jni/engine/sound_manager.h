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

