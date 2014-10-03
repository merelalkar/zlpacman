#pragma once

#include "engine.h"

namespace Pegas
{
	class SpriteAnimation: public Process
	{
	public:
		struct Frame
		{
			float _u0, _v0, _u1, _v1;
			int32 _index, _i, _j;
		};

		enum Mode
		{
			k_modeLooped = 1,
			k_modeBackward = 2
		};

	public:
		SpriteAnimation(RESOURCEID texture, int32 textureSheetWidth, int32 textureSheetHeight, int32 flags = k_modeLooped);
		~SpriteAnimation();

		void setNumFrames(int32 startFrame, int32 numFrames);
		void setFPS(int32 fps);
		void setFlags(int32 flags);

		SpriteParameters* getSprite() { return m_sprite; }
		Frame& getCurrentFrame();			
		
		virtual void update(MILLISECONDS deltaTime);
		
	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

		SpriteParameters* m_sprite;
		int32 m_textureSheetWidth; 
		int32 m_textureSheetHeight;
		int32 m_startFrame; 
		int32 m_numFrames;
		int32 m_fps;
		int32 m_flags;		

		typedef std::list<Frame> FrameList;
		typedef FrameList::iterator	FrameListIt;
		
		FrameList m_frames;
		FrameListIt m_currentFrame;
		MILLISECONDS m_frameLifeTime;
		MILLISECONDS m_ellapsedTime;
	};
}
