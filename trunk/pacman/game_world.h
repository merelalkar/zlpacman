#pragma once

#include "game_objects.h"
#include "tile_grid.h"

namespace Pegas
{
	class Waiting: public Process, public IEventListener 
	{
	public:
		Waiting(float seconds, bool stopOnPause = true);
		virtual void update(MILLISECONDS deltaTime);
		virtual void terminate();

		virtual void handleEvent(EventPtr evt);

		void addFinalEvent(EventPtr evt);

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

	private:
		MILLISECONDS m_remainTime;
		std::list<EventPtr> m_events;
		bool m_stopOnPause;
	};

	class GameWorld: public IEventListener
	{
	public:
		GameWorld();

		virtual void handleEvent(EventPtr evt);

		void create(IPlatformContext* context);
		void destroy(IPlatformContext* context);
		void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
		void render(IPlatformContext* context);
	private:
		typedef SmartPointer<Character> GameObjectPtr;
		typedef std::vector<GameObjectPtr> GameObjectList;
		typedef GameObjectList::iterator GameObjectListIt;
		typedef std::map<TILEID, SpriteParameters> SpriteList;

		void loadMap();
		void createGameObjects();
		void initializeGame();
		void checkNewLife();
		void restartGame();

		IPlatformContext* m_context;

		TileGrid		  m_tileGrid;
		SpriteList		  m_sprites;
		GameObjectList	  m_gameObjects;

		int32 m_currentLevel;
		int32 m_currentScores;
		int32 m_remainPiles;
		int32 m_remainLives;

		int32 m_fragScores;
		int32 m_scoresToLive;
		int32 m_updateScoresToLive;

		ProcessHandle m_superForceTimer1;
		ProcessHandle m_superForceTimer2;

		static const int32 k_scoresForPile;
		static const int32 k_baseScoresForFrag;
		static const int32 k_baseScoresToLife;
		static const int32 k_updateScoresToLife;
		static const int32 k_numLives;
		static const float k_baseSuperForceTime;
	};
}