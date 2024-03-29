/*
 * Types.hpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_TYPES_H
#define PEGAS_TYPES_H

#include <stdint.h>

namespace Pegas
{
	typedef int32_t status;

	const status STATUS_OK = 0;
	const status STATUS_KO = -1;
	const status STATUS_EXIT = -2;


	struct Location
	{
		Location(): mPosX(0.0f), mPosY(0.0f)  {}

		void setPosition(float posX, float posY)
		{
			mPosX = posX;
			mPosY = posY;
		}

		void translate(float cx, float cy)
		{
			mPosX+= cx;
			mPosY+= cy;
		}

		float mPosX;
		float mPosY;
	};
}

#endif /* TYPES_HPP_ */
