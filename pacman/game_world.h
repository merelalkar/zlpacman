#pragma once

#include "game_objects.h"
#include "tile_grid.h"

namespace Pegas
{
	class GameWorld public IEventListener
	{
	public:
		GameWorld();

		void create(IPlatformContext* context);
		void destroy(IPlatformContext* context);
		void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
		void render(IPlatformContext* context);

	private:
		IPlatformContext* m_context;
	};
}