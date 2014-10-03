/*
 * GraphicsSprite.cpp
 *
 *  Created on: 07.09.2014
 *      Author: DNS
 */

#include "graphics_sprite.h"
#include "log.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace Pegas
{
	GraphicsSprite::GraphicsSprite(GraphicsTexture* texture, int32_t width, int32_t height)
		: mTexture(texture), mWidth(width), mHeight(height),
		 mFrameXCount(0), mFrameYCount(0), mFrameCount(0), mAnimationStartFrame(0),
		 mAnimFrameCount(0), mAnimSpeed(0.0f), mAnimFrame(0.0f), mAnimLoop(false)
	{
		Pegas_log_info("GraphicsSprite constructor [texture: %X, width: %d, height: %d]", texture, width, height);
	}

	void GraphicsSprite::load()
	{
		Pegas_log_info("GraphicsSprite::load");

		if(mWidth <= 0)
		{
			mWidth = mTexture->getWidth();
		}

		if(mHeight <= 0)
		{
			mHeight = mTexture->getHeight();
		}

		mFrameXCount = mTexture->getWidth() / mWidth;
		mFrameYCount = mTexture->getHeight() / mHeight;
		mFrameCount = mFrameXCount * mFrameYCount;
	}

	void GraphicsSprite::draw(float timeStep)
	{
		Pegas_log_info_loop("GraphicsSprite::draw, [timeStep: %f]", timeStep);

		int32_t currentFrame, currentFrameX, currentFrameY;
		mAnimFrame += timeStep * mAnimSpeed;

		if(mAnimLoop)
		{
			currentFrame = (mAnimationStartFrame + int32_t(mAnimFrame) % mAnimFrameCount);

		}else if(isAnimationComplete())
		{
			currentFrame = mAnimationStartFrame + (mAnimFrameCount - 1);
		}else
		{
			currentFrame = mAnimationStartFrame + int32_t(mAnimFrame);
		}

		currentFrameX = currentFrame % mFrameXCount;
		currentFrameY = mFrameYCount - 1 - (currentFrame / mFrameXCount);

		mTexture->apply();
		int32_t crop[] = {currentFrameX * mWidth, currentFrameY * mHeight, mWidth,  mHeight };

		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
		glDrawTexfOES(mLocation.mPosX - (mWidth / 2), 	mLocation.mPosY - (mHeight / 2), 0.0f, mWidth, mHeight);
	}

	void GraphicsSprite::setAnimation(int32_t startFrame, int32_t frameCount, float speed, bool loop)
	{
		Pegas_log_info("GraphicsSprite::setAnimation [startFrame: %d, frameCount %d, speed: %f, loop: %d]", startFrame, frameCount, speed, loop);

		mAnimationStartFrame = startFrame;
		mAnimFrame = 0.0f;
		mAnimSpeed = speed;
		mAnimLoop = loop;

		int32_t maxFrameCount = mFrameCount - mAnimationStartFrame;
		mAnimFrameCount = (frameCount > 0 && frameCount <= maxFrameCount) ? frameCount : maxFrameCount;
	}

	bool GraphicsSprite::isAnimationComplete()
	{
		Pegas_log_info_loop("GraphicsSprite::isAnimationComplete");

		return mAnimFrame > (mAnimFrameCount - 1);
	}
}

