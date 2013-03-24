#include "stdafx.h"
#include "game_world.h"

#include "game_resources.h"
#include "pacman_game_screens.h"
#include "fader_layer.h"

#include "platform_windows.h"

using namespace Pegas;

/************************************************************************************************************
	Waiting class
**************************************************************************************************************/
Waiting::Waiting(float seconds, bool stopOnPause)
{
	assert(seconds > 0.0f && "invalid time fo waiting");
	m_remainTime = seconds * 1000.0f;
	m_stopOnPause = stopOnPause;
}

void Waiting::update(MILLISECONDS deltaTime)
{
	m_remainTime-= deltaTime;
	if(m_remainTime <= 0)
	{
		for(std::list<EventPtr>::iterator it = m_events.begin(); it != m_events.end(); ++it)
		{
			TheEventMgr.pushEventToQueye(*it);
		}
		terminate();
	}
}

void Waiting::start(ProcessHandle myHandle, ProcessManagerPtr owner)
{
	Process::start(myHandle, owner);

	if(m_stopOnPause)
	{
		TheEventMgr.addEventListener(this, Event_Game_Pause::k_type);
		TheEventMgr.addEventListener(this, Event_Game_Resume::k_type);
	}
}

void Waiting::terminate()
{
	Process::terminate();

	TheEventMgr.removeEventListener(this);
}

void Waiting::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_Game_Pause::k_type)
	{
		suspend();
	}

	if(evt->getType() == Event_Game_Resume::k_type)
	{
		resume();
	}
}

void Waiting::addFinalEvent(EventPtr evt)
{
	m_events.push_back(evt);
}

	
		

/**************************************************************************************************************
	GameWorld class
***************************************************************************************************************/

const int32 GameWorld::k_scoresForPile = 10;
const int32 GameWorld::k_numLives = 4;
const int32 GameWorld::k_baseScoresForFrag = 200;
const int32 GameWorld::k_baseScoresToLife = 10000;
const int32 GameWorld::k_updateScoresToLife = 2000;
const float GameWorld::k_baseSuperForceTime = 10.0f;
const int32 GameWorld::k_baseScoresForBonus = 100;
const int32 GameWorld::k_baseBonusScoresPerLevel = 200;

GameWorld::GameWorld()
{
	m_superForceTimer1 = 0;
	m_superForceTimer2 = 0;
	m_bonusActive = false;
	m_bonusRow = 0;
	m_bonusColumn = 0;
}

void GameWorld::create(IPlatformContext* context)
{
	m_context = context;

	TheEventMgr.addEventListener(this, Event_CharacterKilled::k_type);
	TheEventMgr.addEventListener(this, Event_PacmanSwallowedPill::k_type);
	TheEventMgr.addEventListener(this, Event_PacmanDeathComplete::k_type);
	TheEventMgr.addEventListener(this, Event_RestartGame::k_type);
	TheEventMgr.addEventListener(this, Event_BonusOn::k_type);
	TheEventMgr.addEventListener(this, Event_BonusOff::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Pause::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Resume::k_type);
	
	loadMap();
	createGameObjects();
	initializeGame();	
}

void GameWorld::destroy(IPlatformContext* context)
{
	for(GameObjectListIt it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->destroy();
	}

	m_tileGrid.destroy();

	TheEventMgr.removeEventListener(this);
}

void GameWorld::loadMap()
{
	int32 canvasWidth = (int32)GrafManager::getInstance().getCanvasWidth();
	int32 canvasHeight = (int32)GrafManager::getInstance().getCanvasHeight();

	tchar fileName[256];
#ifdef		_UNICODE
	wsprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);  
#else
	sprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);
#endif
	
	ISerializer* fileStream = m_context->openFile(fileName, FileOpenMode::k_readOnly);
	if(fileStream)
	{
		OSUtils::getInstance().debugOutput("loading...");

		m_tileGrid.load(*fileStream);
				
		int32 numSprites;
		TILEID id;
		CURCOORD left, top, width, height;
				
		(*fileStream) >> numSprites;
		for(int32 i = 0; i < numSprites; i++)
		{
			(*fileStream) >> id;
			(*fileStream) >> left;
			(*fileStream) >> top;
			(*fileStream) >> width;
			(*fileStream) >> height;

			
			m_sprites[id]._left = left;
			m_sprites[id]._top = top;
			m_sprites[id]._width = width;
			m_sprites[id]._height = height;			
		}//for(int32 i = 0; i < numSprites; i++)				

		m_context->closeFile(fileStream);

		OSUtils::getInstance().debugOutput("loading complete");
	}else
	{
		OSUtils::getInstance().debugOutput("can not open file");
	}

	m_remainPiles = m_tileGrid.getNumTiles(k_tilePill);
}

void GameWorld::createGameObjects()
{
	std::list<Vector3> tiles;
	m_tileGrid.getTiles(k_tilePacman, tiles, true);
	assert(tiles.size() > 0 && "pacman position not found");
	assert(m_sprites.count(k_tilePacman) > 0 && "pacman position not found");

	if(tiles.size() > 0 && m_sprites.count(k_tilePacman) > 0)
	{
		Pacman* pacman = new Pacman(k_actorPacman, m_context);
		pacman->create(&m_tileGrid, tiles.front());

		m_gameObjects.push_back(GameObjectPtr(pacman));

		SpriteAnimation* running = new SpriteAnimation(k_textureCharactersSpriteSheet, 8, 8);
		running->setNumFrames(0, 4);
		running->getSprite()->_width = m_sprites[k_tilePacman]._width;
		running->getSprite()->_height = m_sprites[k_tilePacman]._height;

		pacman->setAnimation(Pacman::k_animationRunning, ProcessPtr(running));

		SpriteAnimation* death = new SpriteAnimation(k_textureCharactersSpriteSheet, 8, 8, 0);
		death->setNumFrames(8, 8);
		death->getSprite()->_width = m_sprites[k_tilePacman]._width;
		death->getSprite()->_height = m_sprites[k_tilePacman]._height;

		pacman->setAnimation(Pacman::k_animationDeath, ProcessPtr(death));
	}
}

void GameWorld::initializeGame()
{
	m_currentLevel = 8;
	m_currentScores = 0;
	m_remainLives = k_numLives;
	m_scoresToLive = k_baseScoresToLife;
	m_updateScoresToLive = k_baseScoresToLife + k_updateScoresToLife;

	EventPtr evtLevel(new Event_HUD_LevelChanged(m_currentLevel));
	TheEventMgr.pushEventToQueye(evtLevel);
	
	EventPtr evtScores(new Event_HUD_ScoresChanged(m_currentScores));
	TheEventMgr.pushEventToQueye(evtScores);

	EventPtr evtLives(new Event_HUD_LivesChanged(m_remainLives));
	TheEventMgr.pushEventToQueye(evtLives);

	EventPtr evtDisableControl(new Event_DisableCharacterControl(k_actorAll));
	TheEventMgr.pushEventToQueye(evtDisableControl);

	EventPtr evtGetReady(new Event_HUD_GetReady());
	TheEventMgr.pushEventToQueye(evtGetReady);
		
	Waiting* waiting = new Waiting(3.0f);
	waiting->addFinalEvent(EventPtr(new Event_RestartGame()));
	m_context->attachProcess(ProcessPtr(waiting));
}

void GameWorld::checkNewLife()
{
	if(m_currentScores >= m_scoresToLive)
	{
		m_remainLives++;
		m_scoresToLive+= m_updateScoresToLive;
		m_updateScoresToLive = k_updateScoresToLife;

		EventPtr evt(new Event_HUD_LivesChanged(m_remainLives));
		TheEventMgr.pushEventToQueye(evt);
	}
}

void GameWorld::restartGame()
{
	EventPtr evt(new Event_HUD_GetReady(false));
	TheEventMgr.pushEventToQueye(evt);

	EventPtr evt2(new Event_EnableCharacterControl(k_actorPacman));
	EventPtr evt3(new Event_EnableCharacterControl(k_actorBlinky));

	TheEventMgr.pushEventToQueye(evt2);
	TheEventMgr.pushEventToQueye(evt3);
	
	Waiting* waiting = new Waiting(2.0f);
	waiting->addFinalEvent(EventPtr(new Event_EnableCharacterControl(k_actorPinky)));
	m_context->attachProcess(ProcessPtr(waiting));
		
	Waiting* waiting2 = new Waiting(2.0f);
	waiting2->addFinalEvent(EventPtr(new Event_EnableCharacterControl(k_actorInky)));
	waiting->attachNext(ProcessPtr(waiting2));
		
	Waiting* waiting3 = new Waiting(2.0f);
	waiting3->addFinalEvent(EventPtr(new Event_EnableCharacterControl(k_actorClyde)));
	waiting2->attachNext(ProcessPtr(waiting3));
}

void GameWorld::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
{
	float fDeltaTime = deltaTime / 1000.0f;
	for(GameObjectListIt it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->update(fDeltaTime);
	}
}

void GameWorld::render(IPlatformContext* context)
{
	m_tileGrid.draw();

	for(GameObjectListIt it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->draw();
	}
}

void GameWorld::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_CharacterKilled::k_type)
	{
		Event_CharacterKilled* pEvent = evt->cast<Event_CharacterKilled>();
		if(pEvent->_actorId == k_actorPacman)
		{
			EventPtr evt(new Event_DisableCharacterControl(k_actorAll));
			TheEventMgr.pushEventToQueye(evt);
			
			Waiting*  waiting = new Waiting(1.0f);
			waiting->addFinalEvent(EventPtr(new Event_PacmanDeath()));
			m_context->attachProcess(ProcessPtr(waiting));
		}else
		{
			m_currentScores+= m_fragScores;
			
			EventPtr evt(new Event_HUD_ScoresChanged(m_currentScores));
			TheEventMgr.pushEventToQueye(evt);
			EventPtr evt2(new Event_HUD_Frag(m_fragScores, pEvent->_position));
			TheEventMgr.pushEventToQueye(evt2);

			m_fragScores*= 2;

			checkNewLife();
		}
	}

	if(evt->getType() == Event_PacmanDeathComplete::k_type)
	{
		m_remainLives--;
		if(m_remainLives < 0)
		{
			EventPtr evt(new Event_HUD_GameOver());
			TheEventMgr.pushEventToQueye(evt);
			
			ProcessPtr waiting(new Waiting(2.0f));

			ProcessPtr fadein(new Fadein());
			waiting->attachNext(fadein);

			ProcessPtr toMainMenu(new ChangeStateTask(m_context, k_stateMainMenu));
			fadein->attachNext(toMainMenu);

			m_context->attachProcess(waiting);
		}else
		{
			EventPtr evt(new Event_HUD_GetReady());
			TheEventMgr.pushEventToQueye(evt);

			EventPtr evt2(new Event_HUD_LivesChanged(m_remainLives));
			TheEventMgr.pushEventToQueye(evt2);

			EventPtr evt3(new Event_ResetActors());
			TheEventMgr.pushEventToQueye(evt3);

			Waiting* waiting = new Waiting(2.0f);
			waiting->addFinalEvent(EventPtr(new Event_RestartGame()));
			m_context->attachProcess(ProcessPtr(waiting));
		}
	}

	if(evt->getType() == Event_RestartGame::k_type)
	{
		restartGame();
	}

	if(evt->getType() == Event_PacmanSwallowedPill::k_type)
	{
		Event_PacmanSwallowedPill* pEvent = evt->cast<Event_PacmanSwallowedPill>();
		if(pEvent->_pill == k_tilePill)
		{
			m_tileGrid.setTile(pEvent->_row, pEvent->_column, TileGrid::k_emptyCellTileId);

			m_currentScores+= k_scoresForPile;
			EventPtr evt(new Event_HUD_ScoresChanged(m_currentScores));
			TheEventMgr.pushEventToQueye(evt);
			checkNewLife();
			
			m_remainPiles--;
			if(m_remainPiles <= 0)
			{
				EventPtr blockControl(new Event_DisableCharacterControl(k_actorAll));
				TheEventMgr.pushEventToQueye(blockControl);

				EventPtr hideCharacters(new Event_HideCharacter(k_actorAll));
				TheEventMgr.pushEventToQueye(hideCharacters);

				EventPtr fadeOn(new Event_GUI_FadeOn(50));
				TheEventMgr.pushEventToQueye(fadeOn);

				loadMap();

				m_currentLevel++;
				EventPtr levelChanged(new Event_HUD_LevelChanged(m_currentLevel));
				TheEventMgr.pushEventToQueye(levelChanged);
				EventPtr newLevel(new Event_HUD_NewLevel(m_currentLevel, 2.0f));
				TheEventMgr.pushEventToQueye(newLevel);
				EventPtr resetActors(new Event_ResetActors());
				TheEventMgr.pushEventToQueye(resetActors);

				Waiting* waiting1 = new Waiting(2.0f);
				waiting1->addFinalEvent(EventPtr(new Event_GUI_FadeOff()));
				waiting1->addFinalEvent(EventPtr(new Event_HUD_GetReady()));
				m_context->attachProcess(ProcessPtr(waiting1));

				Waiting* waiting2 = new Waiting(1.0f);
				waiting2->addFinalEvent(EventPtr(new Event_RestartGame()));
				waiting1->attachNext(ProcessPtr(waiting2));
			}
		}
		if(pEvent->_pill == k_tileSuperPill)
		{
			m_tileGrid.setTile(pEvent->_row, pEvent->_column, TileGrid::k_emptyCellTileId);

			EventPtr evt(new Pegas::Event_SuperForceOn());
			TheEventMgr.pushEventToQueye(evt);

			float superForceTime = k_baseSuperForceTime - (2.0f * (m_currentLevel - 1));
			if(superForceTime <= 0.0f)
			{
				superForceTime = 1.0f;
			}
			float superForcePreTime = superForceTime * 0.8;

			m_context->terminateProcess(m_superForceTimer1);
			m_context->terminateProcess(m_superForceTimer2);
			
			Waiting* waiting1 = new Waiting(superForceTime);
			waiting1->addFinalEvent(EventPtr(new Event_SuperForceOff()));
			m_superForceTimer1 = m_context->attachProcess(ProcessPtr(waiting1));

			Waiting* waiting2 = new Waiting(superForcePreTime);
			waiting2->addFinalEvent(EventPtr(new Event_SuperForcePreOff()));
			m_superForceTimer2 = m_context->attachProcess(ProcessPtr(waiting2));
			
			m_fragScores = k_baseScoresForFrag;
		}
		if(pEvent->_pill == k_tileBonus && m_bonusActive)
		{
			if(pEvent->_row == m_bonusRow && pEvent->_column == m_bonusColumn)
			{
				int32 bonusScores = k_baseScoresForBonus + (k_baseBonusScoresPerLevel * (m_currentLevel - 1));
				m_currentScores+= bonusScores;

				EventPtr evt(new Event_HUD_ScoresChanged(m_currentScores));
				TheEventMgr.pushEventToQueye(evt);

				checkNewLife();

				Vector3 fragPosition;
				m_tileGrid.cellCoords(pEvent->_row, pEvent->_column, fragPosition._x, fragPosition._y, true);

				EventPtr evt2(new Pegas::Event_HUD_Frag(bonusScores, fragPosition));
				TheEventMgr.pushEventToQueye(evt2);
			}
		}
	}

	if(evt->getType() == Event_BonusOn::k_type)
	{
		Event_BonusOn* pEvent = evt->cast<Event_BonusOn>();
		m_bonusActive = true;
		m_bonusRow = pEvent->_row;
		m_bonusColumn = pEvent->_column;
	}

	if(evt->getType() == Event_BonusOff::k_type)
	{
		m_bonusActive = false;
	}

	if(evt->getType() == Event_Game_Pause::k_type)
	{
		EventPtr evt(new Event_DisableCharacterControl(k_actorAll));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(evt->getType() == Event_Game_Resume::k_type)
	{
		EventPtr evt(new Event_EnableCharacterControl(k_actorAll));
		TheEventMgr.pushEventToQueye(evt);
	}	
}