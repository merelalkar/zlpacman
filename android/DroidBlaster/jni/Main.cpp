#include <jni.h>

#include "EventLoop.h"
#include "Context.h"
#include "TimeService.h"
#include "DroidBlaster.h"

void android_main(struct android_app* app)
{
	Pegas::TimeService timeService;
	Pegas::GraphicsService graphicsService(app, &timeService);
	Pegas::Context context = { &graphicsService, &timeService };

	Pegas::EventLoop eventLoop(app);
	dbs::DroidBlaster droidBlaster(context, app);
	eventLoop.run(droidBlaster);
}
