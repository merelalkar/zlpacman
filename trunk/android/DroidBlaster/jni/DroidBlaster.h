/*
 * DroidBlaster.h
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#ifndef DROIDBLASTER_H_
#define DROIDBLASTER_H_

#include "ActivityHandler.h"
#include "Types.h"
#include "TimeService.h"
#include "GraphicsService.h"
#include "Context.h"
#include "Background.h"
#include "Ship.h"

#include <android_native_app_glue.h>

namespace dbs
{
	class DroidBlaster: public Pegas::ActivityHandler
	{
	public:
		DroidBlaster(Pegas::Context& context, android_app* app);
		~DroidBlaster();

	protected:
		virtual Pegas::status onActivate();
		virtual void onDeactivate();
		virtual Pegas::status onStep();

		virtual void onStart();
		virtual void onResume();
		virtual void onPause();
		virtual void onStop();
		virtual void onDestroy();

		virtual void onSaveState(void** pData, int32_t* pSize);
		virtual void onConfigurationChanged();
		virtual void onLowMemory();

		virtual void onCreateWindow();
		virtual void onDestroyWindow();
		virtual void onGainFocus();
		virtual void onLostFocus();

	private:
		Pegas::GraphicsService* 	mGraphicsService;
		Pegas::TimeService*  		mTimeService;
		Ship*  mShip;
		Background* mBackground;
	};

}

#endif /* DROIDBLASTER_H_ */
