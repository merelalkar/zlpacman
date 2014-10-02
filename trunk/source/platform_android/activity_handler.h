/*
 * ActivityHandler.h
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_ACTIVITY_HANDLER_H_
#define PEGAS_ACTIVITY_HANDLER_H_

#include "packt_types.h"

namespace Pegas
{
	class ActivityHandler
	{
	public:
		virtual ~ActivityHandler() {};

		virtual status onActivate() = 0;
		virtual void onDeactivate() = 0;
		virtual status onStep() = 0;

		virtual void onStart() {};
		virtual void onResume() {};
		virtual void onPause() {};
		virtual void onStop() {};
		virtual void onDestroy() {};

		virtual void onSaveState(void** pData, int32_t* pSize) {};
		virtual void onConfigurationChanged() {};
		virtual void onLowMemory() {};

		virtual void onCreateWindow() {};
		virtual void onDestroyWindow() {};
		virtual void onGainFocus() {};
		virtual void onLostFocus() {};
	};
}

#endif /* ACTIVITYHANDLER_H_ */
