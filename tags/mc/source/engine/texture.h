#pragma once

#include "types.h"
#include "smart_pointer.h"

namespace Pegas
{
	class Texture
	{
	public:
		virtual ~Texture() { }
		
		virtual bool create(int32 width, int32 height, 
			int32 format = -1, int32 flags = 0) = 0;
		virtual void destroy() = 0;
		virtual void apply() = 0;

		virtual int32 width() = 0;
		virtual int32 height() = 0;
	};

	typedef SmartPointer<Texture> TexturePtr; 
}