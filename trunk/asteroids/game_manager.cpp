#include "stdafx.h"
#include "game_manager.h"

#include "game_events.h"

namespace Pegas
{
	//TODO: перенести в заголовочный файл
	enum AsteroidLayers
	{
		k_layerMainMenu = 1,
		k_layerOptions,
		k_layerPause,
		k_layerFader,
		k_layerEditor,
		k_layerGameWorld,
		k_layerDebug
	};

	GameManager::GameManager()
		:BaseScreenLayer(_text("game screen"), k_layerGameWorld, false)
	{

	}

	void GameManager::create(IPlatformContext* context)
	{
		TheEventMgr.addEventListener(this, Event_Actor_CreateAsteroid::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateShatter::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateBullet::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateExplosion::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_Destroy::k_type);
	}

	void GameManager::destroy(IPlatformContext* context)
	{
		TheEventMgr.removeEventListener(this);
	}

	void GameManager::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
	{
		m_processes.updateProcesses(deltaTime, timeLimit);
		m_collisionManager.update();

		CollisionManager::CollisionPairList pairList;
		m_collisionManager.getCollidedPairs(pairList);

		for(CollisionManager::CollisionPairList::iterator it = pairList.begin(); it != pairList.end(); ++it)
		{
			CollisionManager::CollisionPair pair = *it;

			assert(m_gameObjects.count(pair.first > 0));
			assert(m_gameObjects.count(pair.second > 0));
			
			IGameObject* o1 = m_gameObjects[pair.first];
			IGameObject* o2 = m_gameObjects[pair.second];

			o1->onCollisionEnter(o2);
			o2->onCollisionEnter(o1);
		}
	}

	void GameManager::render(IPlatformContext* context)
	{
		GrafManager& graph = GrafManager::getInstance();
		for(std::map<ProcessHandle, IGameObject*>::iterator it; it != m_gameObjects.begin(); ++it)
		{
			it->second->onDraw(graph);
		}
	}

	void GameManager::onKeyDown(KeyCode key, KeyFlags flags)
	{
		if(key == IKeyboardController::k_keyCodeUP &&  !(flags & k_keyFlagRepeat))
		{
			EventPtr evt(new Event_Player_Thrust());
			TheEventMgr.pushEventToQueye(evt);
		}

		if(key == IKeyboardController::k_keyCodeLEFT &&  !(flags & k_keyFlagRepeat))
		{
			EventPtr evt(new Event_Player_RotateLeft());
			TheEventMgr.pushEventToQueye(evt);
		}

		if(key == IKeyboardController::k_keyCodeRIGHT &&  !(flags & k_keyFlagRepeat))
		{
			EventPtr evt(new Event_Player_RotateRight());
			TheEventMgr.pushEventToQueye(evt);
		}

		if(key == IKeyboardController::k_keyCodeSPACE &&  !(flags & k_keyFlagRepeat))
		{
			EventPtr evt(new Event_Player_Fire());
			TheEventMgr.pushEventToQueye(evt);
		}

		BaseScreenLayer::onKeyDown(KeyCode key, KeyFlags flags);
	}

	void GameManager::onKeyUp(KeyCode key, KeyFlags flags)
	{
		if(key == IKeyboardController::k_keyCodeUP)
		{
			EventPtr evt(new Event_Player_Stop_Thrust());
			TheEventMgr.pushEventToQueye(evt);
		}

		if(key == IKeyboardController::k_keyCodeLEFT 
			|| key == IKeyboardController::k_keyCodeRIGHT)
		{
			EventPtr evt(new Event_Player_Stop_Rotation());
			TheEventMgr.pushEventToQueye(evt);
		}

		if(key == IKeyboardController::k_keyCodeSPACE)
		{
			EventPtr evt(new Event_Player_Stop_Fire());
			TheEventMgr.pushEventToQueye(evt);
		}

		BaseScreenLayer::onKeyUp(KeyCode key, KeyFlags flags);
	}

	void GameManager::handleEvent(EventPtr evt)
	{
		if(evt->getType() == Event_Actor_CreateAsteroid::k_type)
		{
			Event_Actor_CreateAsteroid* pEvent = evt->cast<Event_Actor_CreateAsteroid>();
			
			Asteroid* asteroid = new Asteroid(&m_collisionManager, pEvent->_position, pEvent->_direction);
			ProcessHandle id = m_processes.attachProcess(ProcessPtr(asteroid));
			m_gameObjects.insert(std::make_pair(id, asteroid));			
			
			return;
		}

		if(evt->getType() == Event_Actor_CreateShatter::k_type)
		{
			Event_Actor_CreateShatter* pEvent = evt->cast<Event_Actor_CreateShatter>();
			
			Shatter* shatter = new Shatter(&m_collisionManager, pEvent->_position, pEvent->_direction);	
			ProcessHandle id = m_processes.attachProcess(ProcessPtr(shatter));
			m_gameObjects.insert(std::make_pair(id, shatter));
			
			return;
		}

		if(evt->getType() == Event_Actor_CreateBullet::k_type)
		{
			Event_Actor_CreateBullet* pEvent = evt->cast<Event_Actor_CreateBullet>();
			
			Bullet* bullet = new Bullet(&m_collisionManager, pEvent->_position, pEvent->_direction);
			ProcessHandle id = m_processes.attachProcess(ProcessPtr(bullet));
			m_gameObjects.insert(std::make_pair(id, bullet));
			
			return;
		}

		if(evt->getType() == Event_Actor_CreateExplosion::k_type)
		{
			Event_Actor_CreateExplosion* pEvent = evt->cast<Event_Actor_CreateExplosion>();
			
			Explosion* explosion = new Explosion(pEvent->_position);
			ProcessHandle id = m_processes.attachProcess(ProcessPtr(explosion));
			m_gameObjects.insert(std::make_pair(id, explosion));
			
			return;
		}

		if(evt->getType() == Event_Actor_Destroy::k_type)
		{
			Event_Actor_Destroy* pEvent = evt->cast<Event_Actor_Destroy>();
			ProcessHandle id = pEvent->_actorID;
			
			m_gameObjects.erase(id);
			m_processes.terminateProcess(id);

			return;
		}
	}
}