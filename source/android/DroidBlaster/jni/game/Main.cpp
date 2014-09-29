#include <jni.h>

#include "EventLoop.h"
#include "Context.h"
#include "TimeService.h"
#include "GraphicsService.h"
#include "SoundService.h"
#include "InputService.h"
#include "DroidBlaster.h"

#include "Log.h"

void android_main(struct android_app* app)
{
	Pegas_log_info("android_main [app: %X]", app);

	Pegas::TimeService timeService;
	Pegas::GraphicsService graphicsService(app, &timeService);
	Pegas::SoundService soundService(app);
	Pegas::InputService inputService(app, graphicsService.getHeight(), graphicsService.getHeight());
	Pegas::Context context = { &graphicsService, &soundService, &inputService, &timeService };

	Pegas::EventLoop eventLoop(app);
	dbs::DroidBlaster droidBlaster(context, app);
	eventLoop.run(&droidBlaster, &inputService);
}
