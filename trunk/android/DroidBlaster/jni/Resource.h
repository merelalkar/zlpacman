/*
 * Resource.h
 *
 *  Created on: 09.08.2014
 *      Author: DNS
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "Types.h"
#include <android_native_app_glue.h>

namespace Pegas
{
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

	private:
		const char* mPath;
		AAssetManager* mAssetManager;
		AAsset* mAsset;
	};
}


#endif /* RESOURCE_H_ */
