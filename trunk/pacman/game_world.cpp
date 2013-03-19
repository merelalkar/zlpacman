#include "stdafx.h"
#include "game_world.h"

#include "fader_layer.h"

using namespace Pegas;

const int32 GameWorld::k_scoresForPile = 10;
const int32 GameWorld::k_numLives = 4;
const int32 GameWorld::k_baseScoresForFrag = 200;
const int32 GameWorld::k_baseScoresToLife = 10000;
const int32 GameWorld::k_updateScoresToLife = 2000;

GameWorld::GameWorld()
{

}

void GameWorld::create(IPlatformContext* context)
{
	TheEventMgr.addEventListener(this, Event_CharacterKilled::k_type);
	TheEventMgr.addEventListener(this, Event_PacmanSwallowedPill::k_type);
	TheEventMgr.addEventListener(this, Event_PacmanDeathComplete::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Pause::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Resume::k_type);

	m_context = context;
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

			if(m_staticSprites.count(id))
			{
				m_sprites[id]._left = left;
				m_sprites[id]._top = top;
				m_sprites[id]._width = width;
				m_sprites[id]._height = height;
			}
		}//for(int32 i = 0; i < numSprites; i++)				

		m_context->closeFile(fileStream);

		OSUtils::getInstance().debugOutput("loading complete");
	}else
	{
		OSUtils::getInstance().debugOutput("can not open file");
	}

	
	std::list<Vector3> tiles;
	m_tileGrid.getTiles(k_tilePacman, tiles, true);
	assert(tiles.size() > 0 && "pacman position not found");

	if(tiles.size() > 0 && m_sprites[id])
	{
		Pacman* pacman = new Pacman(k_tilePacman, context);
		pacman->create(&m_tileGrid, tiles.front());

		m_gameObjects.push_back(GameObjectPtr(pacman));

		SpriteAnimation* running = new SpriteAnimation(k_textureCharactersSpriteSheet, 8, 8);
		running->setNumFrames(0, 4);
		pacman->setAnimation(Pacman::k_animationRunning, ProcessPtr(running));

		SpriteAnimation* death = new SpriteAnimation(k_textureCharactersSpriteSheet, 8, 8, 0);
		death->setNumFrames(8, 8);
		pacman->setAnimation(Pacman::k_animationDeath, ProcessPtr(death));
	}

	m_currentLevel = 1;
	m_currentScores = 0;
	m_remainPiles = m_tileGrid.getNumTiles(k_tilePill);
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

	
	EventPtr evtEnableControlPacman(new Event_EnableCharacterControl(k_actorPacman));
	ProcessPtr waiting1(new Waiting(3.0f, evtEnableControlPacman));
	context->attachProcess(waiting1);

	EventPtr evtEnableControlBlinky(new Event_EnableCharacterControl(k_actorBlinky));
	ProcessPtr waiting2(new Waiting(1.0f, evtEnableControlBlinky));
	waiting1->attachNext(waiting2);

	EventPtr evtEnableControlPinky(new Event_EnableCharacterControl(k_actorPinky));
	ProcessPtr waiting3(new Waiting(1.0f, evtEnableControlPinky));
	waiting2->attachNext(waiting3);

	EventPtr evtEnableControlInky(new Event_EnableCharacterControl(k_actorInky));
	ProcessPtr waiting4(new Waiting(1.0f, evtEnableControlInky));
	waiting3->attachNext(waiting4);

	EventPtr evtEnableControlClyde(new Event_EnableCharacterControl(k_actorClyde));
	ProcessPtr waiting5(new Waiting(1.0f, evtEnableControlClyde));
	waiting4->attachNext(waiting5);
	
	
	EventPtr fadeoutEvent(new Event_GUI_StartFadeout());
	TheEventMgr.pushEventToQueye(fadeoutEvent);
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
			
			EventPtr evt2(new Event_PacmanDeath());
			TheEventMgr.pushEventToQueye(evt2);
		}else
		{
			m_currentScores+= m_fragScores;
			
			EventPtr evt(new Event_HUD_ScoresChanged(m_currentScores));
			TheEventMgr.pushEventToQueye(evt);
			EventPtr evt2(new Pegas::Event_HUD_Frag(m_fragScores, pEvent->_position));
			TheEventMgr.pushEventToQueye(evt2);

			m_fragScores*= 2;

			if(m_currentScores >= m_scoresToLive)
			{
				m_remainLives++;
				m_scoresToLive+= m_updateScoresToLive;
				m_updateScoresToLive = k_updateScoresToLife;

				EventPtr evt3(new Event_HUD_LivesChanged(m_remainLives));
				TheEventMgr.pushEventToQueye(evt3);
			}
		}
	}

	if(evt->getType() == Event_PacmanDeathComplete::k_type)
	{
		ProcessPtr fadein(new Fadein());
		
		m_remainLives--;
		if(m_remainLives < 0)
		{
			EventPtr evt(new Event_HUD_GameOver());
			TheEventMgr.pushEventToQueye(evt);

			ProcessPtr waiting(new Waiting(3.0f, EventPtr());
			waiting->attachNext(fadein);

			ProcessPtr toMainMenu(new ChangeStateTask(m_context, k_stateMainMenu));
			fadein->attachNext(toMainMenu);

			m_context->attachProcess(waiting);
		}else
		{
			EventPtr evt(new Event_ResetActors());
			ProcessPtr waiting(new Waiting(1.5f, evt);
			ProcessPtr fadeout(new Fadeout());

			waiting->attachNext(fadeout);
			m_context->attachProcess(waiting);
			m_context->attachProcess(fadein);
		}
	}

	if(evt->getType() == Event_PacmanSwallowedPill::k_type)
	{

	}

	if(evt->getType() == Event_Game_Pause::k_type)
	{

	}

	if(evt->getType() == Event_Game_Resume::k_type)
	{

	}
}