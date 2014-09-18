/*
 * GraphicsSprite.h
 *
 *  Created on: 07.09.2014
 *      Author: DNS
 */

#ifndef GRAPHICSSPRITE_H_
#define GRAPHICSSPRITE_H_

#include "GraphicsTexture.h"
#include "TimeService.h"
#include "Types.h"

namespace Pegas
{
	class GraphicsSprite
	{
	public:
		GraphicsSprite(GraphicsTexture* texture, int32_t width = 0, int32_t height = 0);

		Location* getLocation() { return &mLocation; }

		void load();
		void draw(float timeStep);
		void setAnimation(int32_t startFrame, int32_t frameCount, float speed, bool loop);
		bool isAnimationComplete();
	private:
		GraphicsTexture* mTexture;
		Location mLocation;
		int32_t mWidth;
		int32_t mHeight;

		int32_t mFrameXCount;
		int32_t mFrameYCount;
		int32_t mFrameCount;
		int32_t mAnimationStartFrame;
		int32_t mAnimFrameCount;
		float mAnimSpeed;
		float mAnimFrame;
		bool mAnimLoop;
	};
}


#endif /* GRAPHICSSPRITE_H_ */
