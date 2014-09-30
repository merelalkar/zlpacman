#include <jni.h>

#include "EventLoop.h"
#include "Context.h"
#include "TimeService.h"
#include "GraphicsService.h"
#include "SoundService.h"
#include "InputService.h"
#include "Sensor.h"
#include "DroidBlaster.h"

#include "Log.h"

void android_main(struct android_app* app)
{
	Pegas_log_info("android_main [app: %X]", app);

	Pegas::EventLoop eventLoop(app);
	Pegas::Sensor accelerometer(eventLoop, ASENSOR_TYPE_ACCELEROMETER);
	Pegas::TimeService timeService;
	Pegas::GraphicsService graphicsService(app, &timeService);
	Pegas::SoundService soundService(app);
	Pegas::InputService inputService(app, &accelerometer, graphicsService.getHeight(), graphicsService.getHeight());
	Pegas::Context context = { &graphicsService, &soundService, &inputService, &timeService };


	dbs::DroidBlaster droidBlaster(context, app);
	eventLoop.run(&droidBlaster, &inputService);
}
