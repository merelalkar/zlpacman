
#include "engine.h"
using namespace Pegas;

ANIMATIONID AnimationManager::s_nextID = 1;

ANIMATIONID AnimationManager::getNextID()
{
	return s_nextID++;
}


ANIMATIONID AnimationManager::registerAnimation(const AnimationParams& params, 
												IAnimationEventListener* pListener)
{
	ANIMATIONID id = getNextID();
	
	m_listeners.insert(std::make_pair(id, pListener));
	m_animations.insert(std::make_pair(id, params));
	m_animations[id]._id = id;
	m_animations[id]._registeringTime = OSUtils::getInstance().getCurrentTime();

	return id;
}

void AnimationManager::unregisterAnimation(const ANIMATIONID& id)
{
	m_animations.erase(id);
	m_listeners.erase(id);
	m_startedAnimations.erase(id);
	
}

void AnimationManager::updateAnimations()
{
	MILLISECONDS currentTime = OSUtils::getInstance().getCurrentTime();
	std::list<ANIMATIONID> markedForErasing;

	for(AnimationsMap::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		AnimationParams& params = it->second;
		ANIMATIONID id = it->first;
		IAnimationEventListener* pListener = m_listeners[id];

		
		if((params._registeringTime + params._delay) <= currentTime)
		{
			if(m_startedAnimations.count(id) == 0)
			{
					pListener->animationStart(params);
					m_startedAnimations.insert(id);
			}
		}else
		{
			continue;
		}
		
		
		if((params._registeringTime + params._delay + params._duration) < currentTime)
		{
			pListener->animationFinish(params);
			if(!params._loop)
			{
				markedForErasing.push_back(id);								
			}

			continue;
		}

		pListener->updateAnimation(params, (currentTime -  (params._registeringTime + params._delay)));
	}

	for(std::list<ANIMATIONID>::iterator it = markedForErasing.begin();
		it != markedForErasing.end(); ++it)
	{
		unregisterAnimation(*it);
	}
}