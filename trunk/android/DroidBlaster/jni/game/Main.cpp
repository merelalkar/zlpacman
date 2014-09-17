#include <jni.h>

#include "../engine/EventLoop.h"
#include "../engine/Context.h"
#include "../engine/TimeService.h"
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
