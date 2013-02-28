#pragma once

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

	public:
		SpriteAnimation(RESOURCEID texture, int32 textureSheetWidth, int32 textureSheetHeight);

		void setNumFrames(int32 startFrame, int32 numFrames);
		void setFPS(int32 fps);

		SpriteParameters* getSprite() { return m_sprite; }
		Frame& getCurrentFrame() { return (*m_currentFrame); }			
		
		virtual void update(MILLISECONDS deltaTime);
		virtual void terminate();

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

		

		SpriteParameters* m_sprite;
		int32 m_textureSheetWidth; 
		int32 m_textureSheetHeight;
		int32 m_startFrame; 
		int32 m_numFrames;
		int32 m_fps;

		

		typedef std::list<Frame> FrameList;
		typedef FrameList::iterator	FrameListIt;

		FrameList m_frames;
		FrameListIt m_currentFrame;
		MILLISECONDS m_frameLifeTime;
		MILLISECONDS m_ellapsedTime;
	};
}