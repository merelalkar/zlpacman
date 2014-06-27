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
		m_numAsteroids = 0;
		m_numShatters = 0;
		m_bDrawCollisions = false;
	}

	void GameManager::create(IPlatformContext* context)
	{
		TheEventMgr.addEventListener(this, Event_Actor_CreateShip::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateAsteroid::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateShatter::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateBullet::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_CreateExplosion::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_Destroy::k_type);
		
		setupWorldBound();
		createShip();
		createAsteroids();	
	}

	void GameManager::setupWorldBound()
	{
		GrafManager& graph = GrafManager::getInstance();
		
		const float span = 10;
		m_worldLeft = -span;
		m_worldRight = graph.getCanvasWidth() + span;
		m_worldTop = -span;
		m_worldBottom = graph.getCanvasHeight() + span;

		m_worldCenter._x = graph.getCanvasWidth() / 2;
		m_worldCenter._y = graph.getCanvasHeight() / 2;
	}

	void GameManager::createShip()
	{
		Vector3 direction(0.0, -1.0, 0.0);
		
		Ship* ship = new Ship(&m_collisionManager, m_worldCenter, direction);
		ProcessHandle id = m_processes.attachProcess(ProcessPtr(ship));
		m_gameObjects.insert(std::make_pair(id, ship));

		EventPtr evt(new Event_Player_EnableControl());
		TheEventMgr.pushEventToQueye(evt);

		EventPtr evt2(new Event_Player_EnableGodMode());
		TheEventMgr.pushEventToQueye(evt2);

		EventPtr evt3(new Event_Player_DisableGodMode());
		Waiting* delayedEvent = new Waiting(4.0f);
		delayedEvent->addFinalEvent(evt3);
		m_processes.attachProcess(ProcessPtr(delayedEvent));
	}

	void GameManager::createAsteroids()
	{
		const int32 numAsteroids = 8;

		GrafManager& graph = GrafManager::getInstance();
		Vector3 position, direction;
		float angle;
		for(int32 i = 0; i < numAsteroids; i++)
		{
			position._x = Math::rand(0.0f, graph.getCanvasWidth());
			position._y = Math::rand(0.0f, graph.getCanvasHeight());
			position._z = 0.0f;

			angle = Math::rand(0.0f, Math::PI * 2.0f);
			direction._x = cos(angle);
			direction._y = sin(angle);
			direction._z = 0.0f;
			direction.normalize();

			EventPtr evt(new Event_Actor_CreateAsteroid(position, direction));
			TheEventMgr.pushEventToQueye(evt);
		}		
	}

	void GameManager::destroy(IPlatformContext* context)
	{
		TheEventMgr.removeEventListener(this);
	}

	void GameManager::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
	{
		m_processes.updateProcesses(deltaTime, timeLimit);
		m_collisionManager.update();

		CollisionManager::CollisionPairList pairList = m_collisionManager.getCollidedPairs();		

		for(CollisionManager::CollisionPairList::iterator it = pairList.begin(); it != pairList.end(); ++it)
		{
			CollisionManager::CollisionPair pair = *it;

			assert(m_gameObjects.count(pair.first)  > 0);
			assert(m_gameObjects.count(pair.second) > 0);
						
			IGameObject* o1 = m_gameObjects[pair.first];
			IGameObject* o2 = m_gameObjects[pair.second];

			o1->onCollisionEnter(o2);
			o2->onCollisionEnter(o1);
		}

		for(std::map<ProcessHandle, IGameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
		{
			Vector3 position = it->second->getPosition();
			if(!isObjectVisible(position))
			{
				GameObjectType got = it->second->getType();

				if(got == "Asteroid" || got == "Ship" ||  got == "Shatter")
				{
					int32 actorID = it->first;
					Vector3 newPosition = getPositionToTeleport(position);

					EventPtr evt(new Event_Actor_SetPosition(actorID, newPosition));
					TheEventMgr.pushEventToQueye(evt);
				}

				if(got == "Bullet")
				{
					int32 actorID = it->first;

					EventPtr evt(new Event_Actor_Destroy(actorID));
					TheEventMgr.pushEventToQueye(evt);
				}
			}			
		}//for(std::map<ProcessHandle, IGameObject*>::iterator it = m_gameObjects.begin();
	}

	void GameManager::render(IPlatformContext* context)
	{
		GrafManager& graph = GrafManager::getInstance();
		for(std::map<ProcessHandle, IGameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
		{
			it->second->onDraw(graph);
		}

		if(m_bDrawCollisions)
		{
			m_collisionManager.debugDraw();
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

		if(key == IKeyboardController::k_keyCode_T &&  !(flags & k_keyFlagRepeat))
		{
			m_bDrawCollisions = (!m_bDrawCollisions);
		}

		BaseScreenLayer::onKeyDown(key, flags);
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

		BaseScreenLayer::onKeyUp(key, flags);
	}

	void GameManager::handleEvent(EventPtr evt)
	{
		if(evt->getType() == Event_Actor_CreateAsteroid::k_type)
		{
			Event_Actor_CreateAsteroid* pEvent = evt->cast<Event_Actor_CreateAsteroid>();
			
			Asteroid* asteroid = new Asteroid(&m_collisionManager, pEvent->_position, pEvent->_direction);
			ProcessHandle id = m_processes.attachProcess(ProcessPtr(asteroid));
			m_gameObjects.insert(std::make_pair(id, asteroid));

			m_numAsteroids++;
			
			return;
		}

		if(evt->getType() == Event_Actor_CreateShatter::k_type)
		{
			Event_Actor_CreateShatter* pEvent = evt->cast<Event_Actor_CreateShatter>();
			
			Shatter* shatter = new Shatter(&m_collisionManager, pEvent->_position, pEvent->_direction);	
			ProcessHandle id = m_processes.attachProcess(ProcessPtr(shatter));
			m_gameObjects.insert(std::make_pair(id, shatter));
			
			m_numShatters++;

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

			if(m_gameObjects.count(id) > 0)
			{
				if(m_gameObjects[id]->getType() == "Ship")
				{
					//TODO: decrement life scores;

					//TODO: if not zero:
					EventPtr evt2(new Event_Actor_CreateShip());
					Waiting* delayedEvent = new Waiting(3.0f);
					delayedEvent->addFinalEvent(evt2);
					m_processes.attachProcess(ProcessPtr(delayedEvent));
				}

				if(m_gameObjects[id]->getType() == "Asteroid")
				{
					m_numAsteroids--;
					
				}

				if(m_gameObjects[id]->getType() == "Shatter")
				{
					m_numShatters--;
				}

				if(m_numAsteroids <= 0 && m_numShatters <= 0)
				{
					EventPtr evt2(new Event_Player_EnableGodMode());
					TheEventMgr.pushEventToQueye(evt2);

					EventPtr evt3(new Event_Player_DisableGodMode());
					Waiting* delayedEvent = new Waiting(4.0f);
					delayedEvent->addFinalEvent(evt3);
					m_processes.attachProcess(ProcessPtr(delayedEvent));
						
					createAsteroids();
				}
			}
			
			m_gameObjects.erase(id);
			m_processes.terminateProcess(id);

			return;
		}

		if(evt->getType() == Event_Actor_CreateShip::k_type)
		{
			createShip();
			return;
		}
	}

	bool GameManager::isObjectVisible(const Vector3& position)
	{
		if(position._x > m_worldRight)
			return false;
		if(position._x < m_worldLeft)
			return false;
		if(position._y > m_worldBottom)
			return false;
		if(position._y < m_worldTop)
			return false;

		return true;
	}

	Vector3 GameManager::getPositionToTeleport(const Vector3& position)
	{
		Vector3 offset = position - m_worldCenter;
		offset = offset * -1.0;
		
		Vector3 result;
		do
		{
			result = m_worldCenter + offset;
			offset = offset * 0.9;
		}while(!isObjectVisible(result));

		return result;
	}
}