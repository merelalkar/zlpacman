#include "sprite_animation.h"

namespace Pegas
{
	SpriteAnimation::SpriteAnimation(RESOURCEID texture, int32 textureSheetWidth, int32 textureSheetHeight, int32 flags)
	{
		m_sprite = new SpriteParameters();
		m_sprite->_texture = texture;

		m_textureSheetWidth = textureSheetWidth;
		m_textureSheetHeight = textureSheetHeight;
		m_startFrame = 0;
		m_numFrames = textureSheetWidth * textureSheetHeight;
		m_fps =  32;
		m_frameLifeTime = (MILLISECONDS)(1000.0 / m_fps);
		m_ellapsedTime = 0;
		m_flags = flags;
	}

	SpriteAnimation::~SpriteAnimation()
	{
		if(m_sprite)
		{
			delete m_sprite;
			m_sprite = 0;
		}
	}

	void SpriteAnimation::setNumFrames(int32 startFrame, int32 numFrames)
	{
		m_startFrame = startFrame;
		m_numFrames = numFrames;
	}

	void SpriteAnimation::setFPS(int32 fps)
	{
		m_fps = fps;
		m_frameLifeTime = (MILLISECONDS)(1000.0 / m_fps);
		m_ellapsedTime = 0;
	}

	void SpriteAnimation::setFlags(int32 flags)
	{
		m_flags = flags;
	}

	SpriteAnimation::Frame& SpriteAnimation::getCurrentFrame()
	{
		if(m_currentFrame != m_frames.end())
		{
			return (*m_currentFrame);
		}

		return m_frames.back();
	}

	void SpriteAnimation::update(MILLISECONDS deltaTime)
	{
		m_ellapsedTime+= deltaTime;
		if(m_ellapsedTime >= m_frameLifeTime)
		{
			m_ellapsedTime = 0;

			++m_currentFrame;
			if(m_currentFrame == m_frames.end())
			{
				if(m_flags & k_modeLooped)
				{
					m_currentFrame = m_frames.begin();
				}else
				{
					terminate();
					return;
				}
			}
		}

		if(m_currentFrame != m_frames.end())
		{
			m_sprite->_minU = (*m_currentFrame)._u0;
			m_sprite->_maxU = (*m_currentFrame)._u1;
			m_sprite->_minV = (*m_currentFrame)._v0;
			m_sprite->_maxV = (*m_currentFrame)._v1;

			m_sprite->_flags |= k_customTextureCoords;
		}
	}

	void SpriteAnimation::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		m_frames.clear();
	/*
		TextureResource* pTexture = TextureResourceManager::getInstance().getResource(m_sprite->_texture);

		float _pitchU = pTexture->getMaxUCoord() / m_textureSheetWidth;
		float _pitchV = pTexture->getMaxVCoord() / m_textureSheetHeight;

		int32 num = 0;
		for(int32 j = 0; j < m_textureSheetHeight; j++)
		{
			for(int32 i = 0; i < m_textureSheetWidth; i++)
			{
				int32 index = (j * m_textureSheetWidth) + i;
				if(index < m_startFrame)
					continue;
				if(index >= (m_startFrame + m_numFrames))
					break;

				Frame frame;
				frame._u0 = _pitchU * i;
				frame._u1 = frame._u0 + _pitchU;
				frame._v0 = pTexture->getMaxVCoord() - (_pitchV * (j + 1));
				frame._v1 = frame._v0 + _pitchV;

				frame._index = num;
				frame._i = i;
				frame._j = j;

				m_frames.push_back(frame);
				num++;

				if(num >= m_numFrames)
					break;
			}//for(int32 j = 0; j < m_textureSheetWidth; j++)
		}//for(int32 i = 0; i < m_textureSheetWidth; i++)

		m_currentFrame = m_frames.begin();
		m_ellapsedTime = 0;*/
	}
}
