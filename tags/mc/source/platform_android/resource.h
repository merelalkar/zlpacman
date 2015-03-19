/*
 * Resource.h
 *
 *  Created on: 09.08.2014
 *      Author: DNS
 */

#pragma once

#include "packt_types.h"

#include <android_native_app_glue.h>

namespace packt
{
	struct ResourceDescriptor
	{
		int32_t mDescriptor;
		off_t mStart;
		off_t mLength;
	};

	class Resource
	{
	public:
		class AutoDispose
		{
		public:
			AutoDispose(Resource& r): mResource(r) {}
			~AutoDispose() { mResource.close(); }
		private:
			Resource& mResource;
		};
	public:
		Resource(android_app* app, const char* path);

		const char* getPath();

		status open();
		void close();
		status read(void* buffer, size_t count);

		off_t getLength();
		const void* bufferize();
		ResourceDescriptor descript();

	private:
		const char* mPath;
		AAssetManager* mAssetManager;
		AAsset* mAsset;
	};
}



