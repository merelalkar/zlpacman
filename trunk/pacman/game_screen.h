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
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void handleEvent(EventPtr evt);

	private:
		GameWorld m_gameWorld;

		SpriteParameters m_maze;
		SpriteParameters m_scoresTextSprite;
		SpriteParameters m_livesText;
		SpriteParameters m_liveIcon;
		
		TextParameters	 m_scoresTextParams;
		String			 m_scoresText;
		TextParameters	 m_levelTextParams;
		String			 m_levelText;

		int32 m_numLives;
		int32 m_numScore;
		int32 m_level;
	};

	class GameScreen: public DefaultGameState, public IEventListener
	{
	public:
		GameScreen();
		virtual void enter(IPlatformContext* context);
		virtual void leave(IPlatformContext* context);
		virtual void handleEvent(EventPtr evt);
	};
}