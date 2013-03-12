#pragma once

#include "default_game_state.h"
#include "game_world.h"

namespace Pegas
{
	class GameVerticalLayer: public BaseScreenLayer, public IEventListener
	{
	public:
		GameVerticalLayer();

		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);
		virtual void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
		virtual void render(IPlatformContext* context);	
		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void handleEvent(EventPtr evt);

	private:
		GameWorld m_gameWorld;

		SpriteParameters m_maze;
		SpriteParameters m_scoresText;
		SpriteParameters m_livesText;
		SpriteParameters m_liveIcon;
		TextParameters m_scoresText;
		int32 m_numLives;
	};	
}