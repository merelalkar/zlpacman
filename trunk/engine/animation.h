#ifndef PEGAS_ANIMATION_H
#define PEGAS_ANIMATION_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{
	

	struct AnimationParams
	{
		ANIMATIONID _id;
		MILLISECONDS _delay;
		MILLISECONDS _duration;
		MILLISECONDS _registeringTime;
		bool _loop;
	};

	class IAnimationEventListener
	{
	public:
		virtual void animationStart(const AnimationParams& params) = 0;
		virtual void updateAnimation(const AnimationParams& params, MILLISECONDS elapsedTime) = 0;
		virtual void animationFinish(const AnimationParams& params) = 0;
	};
	
	class AnimationManager: public Singleton<AnimationManager>
	{
	public:
		typedef std::map<ANIMATIONID, IAnimationEventListener*> ListenersMap;
		typedef std::map<ANIMATIONID, AnimationParams> AnimationsMap;
		
		AnimationManager(): Singleton(*this) {};
		virtual ~AnimationManager() {};

		ANIMATIONID registerAnimation(const AnimationParams& params, IAnimationEventListener* pListener);
		void unregisterAnimation(const ANIMATIONID& id);

		void updateAnimations();

	protected:
		ListenersMap m_listeners;
		AnimationsMap m_animations;
		std::set<ANIMATIONID> m_startedAnimations;

		static ANIMATIONID getNextID();
		static ANIMATIONID s_nextID;		
	};
}

#endif //PEGAS_ANIMATION_H