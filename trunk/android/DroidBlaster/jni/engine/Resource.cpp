/*
 * Resource.cpp
 *
 *  Created on: 10.08.2014
 *      Author: DNS
 */

#include "Resource.h"
#include "Log.h"

namespace Pegas
{
	Resource::Resource(android_app* app, const char* path)
		:mPath(path), mAssetManager(app->activity->assetManager), mAsset(NULL)
	{
		Pegas_log_debug("Resource::constructor [app: %X, path: %s]", app, path);
	}

	const char* Resource::getPath()
	{
		Pegas_log_debug("Resource::getPath");

		return mPath;
	}

	status Resource::open()
	{
		Pegas_log_debug("Resource::open");

		mAsset = AAssetManager_open(mAssetManager, mPath, AASSET_MODE_UNKNOWN);

		return (mAsset != NULL) ? STATUS_OK : STATUS_KO;
	}

	void Resource::close()
	{
		Pegas_log_debug("Resource::close");

		if (mAsset != NULL)
		{
			AAsset_close(mAsset);
			mAsset = NULL;
		}
	}

	status Resource::read(void* buffer, size_t count)
	{
		Pegas_log_debug("Resource::read [buffer: %X, count: %d]", buffer, count);
		int32_t lRead = AAsset_read(mAsset, buffer, count);

		return (lRead == count) ? STATUS_OK : STATUS_KO;
	}

	off_t Resource::getLength()
	{
		return AAsset_getLength(mAsset);
	}

	const void* Resource::bufferize()
	{
		return AAsset_getBuffer(mAsset);
	}
}
