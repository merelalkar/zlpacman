
#include "platform_windows.h"
using namespace Pegas;

void WinMMSoundManager::playSound(RESOURCEID soundID)
{
	SoundResource* pSound = SoundResourceManager::getInstance().getResource(soundID);
	IntResourceCode& code = pSound->getCode();

	PlaySound(MAKEINTRESOURCE(code.resourceID), code.module, SND_RESOURCE|SND_ASYNC);
}