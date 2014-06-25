#include "stdafx.h"
#include "game_objects.h"

#include "game_events.h"

namespace Pegas
{
	//TODO: tweak constants
	const int32 k_asteroidNumPoints = 7;
	const int32 k_shatterNumPoints = 5;

	const float k_asteroidVelosity = 50.0f;
	const float k_bulletVelocity = 400.0f;
	const float k_shipVelocity = 100.0f;

	const float k_asteroidRadius = 30.0f;
	const float k_shatterRadius = 10.0f;
	const float k_bulletSize = 5.0f;

	const int32 k_asteroidCollisionGroup = 1;
	const int32 k_shipCollisionGroup = 2;
	const int32 k_bulletCollisionGroup = 4;
	const int32	k_borderCollisionGroup = 8;

	const float k_shipLinerVelocity = 2.0f;
	const float k_shipAngleVelocity = 5.0f;
	const float k_shipLinearSpeedAcceleration = 150.0f;
	const float k_shipLinearSpeedAttenuation = 20.0f;

	const Vector3 k_shipDefaultDirection(0.0f, -1.0f, 0.0f);

	const float k_shotInterval = 0.1f;
	const float k_flamePhaseTime = 0.3f;

	const int32 k_minExplosionParticles = 10;
	const int32 k_maxExplosionParticles = 30;
	const float k_initialExplosionRadius = 10.0f;
	const float k_particleVelocity = 4.0f;
	const float k_particleHalfSize = 1.5f;
	const float k_particleSize = 3.0f;
	const float k_explosionLifeTime = 3.0f;

	
	// Generate a random number between 0 and 1
	// return a uniform number in [0,1].
	float unifRand()
	{
		return rand() / float(RAND_MAX);
	}
	
	// Generate a random number in a real interval.
	// param a one end point of the interval
	// param b the other end of the interval
	// return a inform rand numberin [a,b].
	float unifRand(float a, float b)
	{
		return (b-a) * unifRand() + a;
	}

	const float _PI = 3.14159265f;

	/********************************************************************************
		Asteroid class implementation
	*********************************************************************************/
	Asteroid::Asteroid(CollisionManager* collisionManager, const Vector3& position, const Vector3& direction)
	{
		m_collisionManager = collisionManager;
		m_position = position;
		m_direction = direction;
	}

	void Asteroid::generatePoints(int32 numPoints, float maxRadius, std::vector<Vector3>& points)
	{
		assert(numPoints > 0);
		assert(maxRadius > 0.0f);

		float angleStep = (2.0f * _PI) / numPoints;
		float angle = unifRand(0.0f, _PI);
		float minRadius = maxRadius * 0.5f;

		points.reserve(numPoints);
		for(int32 i = 0; i < numPoints; i++)
		{
			Vector3 point;
			
			float radius = unifRand(minRadius, maxRadius);
			point._x = radius * cos(angle);
			point._y = radius * sin(angle);

			points.push_back(point);
			angle+= angleStep;
		}
	}

	void Asteroid::movePoints(const Vector3& offset)
	{
		for(int32 i = 0; i < m_points.size(); i++)
		{
			m_points[i]= m_points[i] + offset;
		}
	}

	void Asteroid::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		TheEventMgr.addEventListener(this, Event_Actor_SetPoisition::k_type);

		generatePoints(k_asteroidNumPoints, k_asteroidRadius, m_points);
		movePoints(m_position);

		m_collisionManager->registerCircle(myHandle, k_asteroidCollisionGroup, m_position, k_asteroidRadius);
	}

	void Asteroid::update(MILLISECONDS deltaTime)
	{
		Vector3 offset = m_direction * k_asteroidVelosity * (deltaTime / 1000.0f);
		m_position = m_position + offset;

		movePoints(offset);
		m_collisionManager->moveObject(m_handle, offset, false);
	}

	void Asteroid::terminate()
	{
		TheEventMgr.removeEventListener(this);
		m_collisionManager->unregisterCollisionHull(m_handle);
	
		Process::terminate();
	}

	void Asteroid::handleEvent(EventPtr evt)
	{
		if(evt->getType() == Event_Actor_SetPoisition::k_type)
		{
			Event_Actor_SetPoisition* evt2 = evt->cast<Event_Actor_SetPoisition>();
			if(evt2->_actorID == m_handle)
			{
				Vector3 offset = evt2->_position - m_position;
				m_position = evt2->_position;

				movePoints(offset);
				m_collisionManager->moveObject(m_handle, offset, false);
			}
			return;
		}
	}

	void Asteroid::onCollisionEnter(IGameObject* other)
	{
		if(other->getType() == "Bullet")
		{
			createShatters();

			EventPtr evt(new Event_Actor_Destroy(m_handle));
			TheEventMgr.pushEventToQueye(evt);
		}
	}

	void Asteroid::onDraw(GrafManager& graphManager)
	{
		CURCOORD fromX, fromY, toX, toY;
		RGBCOLOR color = 0xffffffff;

		for(int32 i = 1; i < m_points.size(); i++)
		{
			fromX = m_points[i - 1]._x;
			fromY = m_points[i - 1]._y;
			toX = m_points[i]._x;
			toY = m_points[i]._y;

			graphManager.drawLine(fromX, fromY, toX, toY, color);
		}

		int32 iLast = m_points.size() - 1;

		fromX = m_points[0]._x;
		fromY = m_points[0]._y;
		toX = m_points[iLast]._x;
		toY = m_points[iLast]._y;

		graphManager.drawLine(fromX, fromY, toX, toY, color);
	}

	void Asteroid::createShatters()
	{
		const int32 minShatters = 3;
		const int32 maxShatters = 7;

		int32 numShatters = minShatters + (rand() % maxShatters);
		if(numShatters > maxShatters)
		{
			numShatters = maxShatters;
		}

		float angleStep = (2.0f * _PI) / numShatters;
		float angle = unifRand(0.0f, _PI);
		
		for(int32 i = 0; i < numShatters; i++)
		{
			Vector3 point, direction;
			
			float radius = unifRand(0.0f, k_asteroidRadius);
			point._x = radius * cos(angle);
			point._y = radius * sin(angle);

			direction = point - m_position;
			direction.normalize();

			angle+= angleStep;

			EventPtr evt(new Event_Actor_CreateShatter(point, direction));
			TheEventMgr.pushEventToQueye(evt);
		}
	}

	/******************************************************************************************************
		Shatter class implementation
	*******************************************************************************************************/
	Shatter::Shatter(CollisionManager* collisionManager, const Vector3& position, const Vector3& direction)
		:Asteroid(collisionManager, position, direction)
	{

	}

	void Shatter::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		generatePoints(k_shatterNumPoints, k_shatterRadius, m_points);
		movePoints(m_position);

		m_collisionManager->registerCircle(myHandle, k_asteroidCollisionGroup, m_position, k_shatterRadius);
	}

	void Shatter::onCollisionEnter(IGameObject* other)
	{
		if(other->getType() == "Bullet")
		{
			EventPtr evt(new Event_Actor_CreateExplosion(m_position));
			TheEventMgr.pushEventToQueye(evt);

			EventPtr evt2(new Event_Actor_Destroy(m_handle));
			TheEventMgr.pushEventToQueye(evt2);
		}
	}

	/****************************************************************************************************
		Bullet class implementation
	***************************************************************************************************/
	Bullet::Bullet(CollisionManager* collisionManager, const Vector3& position, const Vector3& direction)
	{
		m_collisionManager = collisionManager;
		m_position = position;
		m_direction = direction;
	}

	void Bullet::onCollisionEnter(IGameObject* other)
	{
		/*EventPtr evt(new Event_Actor_Destroy(m_handle));
		TheEventMgr.pushEventToQueye(evt);*/
	}

	void Bullet::onDraw(GrafManager& graphManager)
	{
		CURCOORD fromX, fromY, toX, toY;
		RGBCOLOR color = 0xffffffff;

		Vector3 to = m_position + m_direction * k_bulletSize;

		fromX = m_position._x;
		fromY = m_position._y;
		toX = to._x;
		toY = to._y;

		graphManager.drawLine(fromX, fromY, toX, toY, color);
	}

	void Bullet::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);
		
		m_collisionManager->registerPoint(myHandle, k_bulletCollisionGroup, m_position);
	}

	void Bullet::update(MILLISECONDS deltaTime)
	{
		Vector3 offset = m_direction * k_bulletVelocity * (deltaTime / 1000.0f);
		m_position = m_position + offset;

		m_collisionManager->moveObject(m_handle, offset, false);	
	}

	void Bullet::terminate()
	{
		m_collisionManager->unregisterCollisionHull(m_handle);

		Process::terminate();
	}

	/*****************************************************************************************
		Ship class implementation	
	*****************************************************************************************/
	
	Ship::Ship(CollisionManager* collisionManager, const Vector3& position, const Vector3& direction)
	{
		m_collisionManager = collisionManager;
		m_position = position;
		m_direction = direction;
		
		m_direction.normalize();

		float cosA = m_direction.dotProduct(k_shipDefaultDirection);

		m_rotation = acos(cosA); 
		m_velocity = Vector3(0.0f, 0.0f, 0.0f);
		m_nRotation = k_noRotation;	
		m_bThrusted = false;
		m_bFireOn = false;
		m_enabled = false;

		m_currentFlamePhase = 0;
	}

	void Ship::transformPoints()
	{
		Matrix4x4 translation, rotation, world;
		
		rotation.identity();
		rotation.rotateZ(m_rotation);

		translation.identity();
		translation.translate(m_position._x, m_position._y, 0.0f);
		
		world = rotation * translation;

		for(int32 i = 0; i < m_initialPoints.size(); i++)
		{
			m_points[i] = m_initialPoints[i] * world;
		}

		m_direction = k_shipDefaultDirection * rotation; 

		m_collisionManager->rotateObject(m_handle, m_rotation);
		m_collisionManager->moveObject(m_handle, m_position); 
	}

	void Ship::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		TheEventMgr.addEventListener(this, Event_Player_EnableControl::k_type);
		TheEventMgr.addEventListener(this, Event_Player_DisableControl::k_type);
		TheEventMgr.addEventListener(this, Event_Player_RotateLeft::k_type);
		TheEventMgr.addEventListener(this, Event_Player_RotateRight::k_type);
		TheEventMgr.addEventListener(this, Event_Player_Thrust::k_type);
		TheEventMgr.addEventListener(this, Event_Player_Fire::k_type);
		TheEventMgr.addEventListener(this, Event_Player_Stop_Thrust::k_type);
		TheEventMgr.addEventListener(this, Event_Player_Stop_Fire::k_type);
		TheEventMgr.addEventListener(this, Event_Player_Stop_Rotation::k_type);
		TheEventMgr.addEventListener(this, Event_Actor_SetPoisition::k_type);

		m_initialPoints.push_back(Vector3(18.0f, 4.0f, 0.0f));
		m_initialPoints.push_back(Vector3(7.0f, 31.0f, 0.0f));
		m_initialPoints.push_back(Vector3(30.0f, 31.0f, 0.0f));
		m_initialPoints.push_back(Vector3(19.0f, 21.0f, 0.0f));
		m_initialPoints.push_back(Vector3(12.0f, 27.0f, 0.0f));
		m_initialPoints.push_back(Vector3(24.0f, 26.0f, 0.0f));
		m_initialPoints.push_back(Vector3(17.0f, 32.0f, 0.0f));
		m_initialPoints.push_back(Vector3(17.0f, 38.0f, 0.0f));
		m_initialPoints.push_back(Vector3(18.0f, 4.0f, 0.0f));

		m_spawnBulletPoint = m_initialPoints.size() - 1; 

		//initial scalling and translating of points;
		Matrix4x4 translation;
		translation.identity();
		translation.translate(-m_initialPoints[3]._x, -m_initialPoints[3]._y, 0.0f);
		
		for(int32 i = 0; i < m_initialPoints.size(); i++)
		{
			m_initialPoints[i] = m_initialPoints[i] * translation;
		}

		m_points.insert(m_points.begin(), m_initialPoints.begin(), m_initialPoints.end());
		
		m_collisionManager->registerPoligon(myHandle, k_shipCollisionGroup, 
			CollisionManager::PointList(m_points.begin(), m_points.begin() + 3));

		transformPoints();
				
		m_corpusIndices.push_back(std::make_pair(0, 1));
		m_corpusIndices.push_back(std::make_pair(1, 3));
		m_corpusIndices.push_back(std::make_pair(3, 2));
		m_corpusIndices.push_back(std::make_pair(2, 0));

		m_flameIndices[0].push_back(std::make_pair(4, 6));
		m_flameIndices[0].push_back(std::make_pair(6, 5));

		m_flameIndices[1].push_back(std::make_pair(4, 7));
		m_flameIndices[1].push_back(std::make_pair(7, 5));		
	}

	void Ship::update(MILLISECONDS deltaTime)
	{
		float dt = deltaTime / 1000.0f;

		if(m_bThrusted)
		{
			m_velocity = m_velocity + (m_direction * k_shipLinearSpeedAcceleration * dt);
			m_thrustTime+= dt;

			if(m_thrustTime >= k_flamePhaseTime)
			{
				m_thrustTime = 0.0f;
				m_currentFlamePhase = 1 - m_currentFlamePhase;
			}			
		}
		
		/*if(!m_bThrusted && m_velocity.length() > 1.0f)
		{
			Vector3 normalSpeed = m_velocity;
			normalSpeed.normalize();

			m_velocity = m_velocity - (normalSpeed * k_shipLinearSpeedAttenuation * dt);
			if(m_velocity.length() < 1.0f)
			{
				m_velocity = Vector3(0.0, 0.0, 0.0);
			}
		}*/

		m_position = m_position + (m_velocity * dt);

		if(m_nRotation == k_rotationLeft)
			m_rotation+= k_shipAngleVelocity * dt;

		if(m_nRotation == k_rotationRight)
			m_rotation-= k_shipAngleVelocity * dt;

		transformPoints();

		if(m_bFireOn)
		{
			m_lastShotTime+= dt;
			if(m_lastShotTime >= k_shotInterval)
			{
				m_lastShotTime = 0.0f;

				Vector3 position = m_points[m_spawnBulletPoint];
				EventPtr evt(new Event_Actor_CreateBullet(position, m_direction));
				TheEventMgr.pushEventToQueye(evt);
			}
		}
	}

	void Ship::terminate()
	{
		TheEventMgr.removeEventListener(this);
		m_collisionManager->unregisterCollisionHull(m_handle);

		Process::terminate();
	}
		
	void Ship::handleEvent(EventPtr evt)
	{
		if(m_enabled && evt->getType() == Event_Player_Fire::k_type)
		{
			m_bFireOn = true;
			m_lastShotTime = k_shotInterval;

			return;
		}

		if(m_enabled && evt->getType() == Event_Player_Thrust::k_type)
		{
			m_bThrusted = true;
			m_thrustTime = 0.0f;

			return;
		}

		if(m_enabled && evt->getType() == Event_Player_RotateLeft::k_type)
		{
			m_nRotation = k_rotationLeft;
			return;
		}

		if(m_enabled && evt->getType() == Event_Player_RotateRight::k_type)
		{
			m_nRotation = k_rotationRight;
			return;
		}

		if(evt->getType() == Event_Player_Stop_Fire::k_type)
		{
			m_bFireOn = false;
			return;
		}

		if(evt->getType() == Event_Player_Stop_Thrust::k_type)
		{
			m_bThrusted = false;
			return;
		}

		if(evt->getType() == Event_Player_Stop_Rotation::k_type)
		{
			m_nRotation = k_noRotation;
			return;
		}

		if(evt->getType() == Event_Player_EnableControl::k_type)
		{
			m_enabled = true;
			return;
		}

		if(evt->getType() == Event_Player_DisableControl::k_type)
		{
			m_enabled = false;
			return;
		}

		if(evt->getType() == Event_Actor_SetPoisition::k_type)
		{
			Event_Actor_SetPoisition* evt2 = evt->cast<Event_Actor_SetPoisition>();
			if(evt2->_actorID == m_handle)
			{
				m_position = evt2->_position;
				transformPoints();
			}
			return;
		}
	}

	void Ship::onCollisionEnter(IGameObject* other)
	{
		if(other->getType() == "Asteroid" || other->getType() == "Shatter")
		{
			EventPtr evt(new Event_Actor_CreateExplosion(m_position));
			TheEventMgr.pushEventToQueye(evt);

			EventPtr evt2(new Event_Actor_Destroy(m_handle));
			TheEventMgr.pushEventToQueye(evt2);
		}
	}

	void Ship::onDraw(GrafManager& graphManager)
	{
		for(std::vector<std::pair<int32, int32> >::iterator it = m_corpusIndices.begin(); it != m_corpusIndices.end(); ++it)
		{
			Vector3 from = m_points[it->first];
			Vector3 to = m_points[it->second];
			
			GrafManager::getInstance().drawLine(from._x, from._y, to._x, to._y, 0xffffffff);
		}

		if(m_bThrusted)
		{
			for(std::vector<std::pair<int32, int32> >::iterator it = m_flameIndices[m_currentFlamePhase].begin(); 
				it != m_flameIndices[m_currentFlamePhase].end(); ++it)
			{
				Vector3 from = m_points[it->first];
				Vector3 to = m_points[it->second];
			
				GrafManager::getInstance().drawLine(from._x, from._y, to._x, to._y, 0xffffffff);

			}//for(std::vector<std::pair<int32, int32> >::iterator it = m_cor			
		}//if(m_bThrusted)
	}

	/*******************************************************************************************************************
		Explosion game object
	********************************************************************************************************************/
	Explosion::Explosion(const Vector3& position)
	{
		m_position = position;
		m_lifeTime = 0.0f;
	}
		
	void Explosion::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		m_lifeTime = 0.0f;
		
		int32 numParticles = k_minExplosionParticles + rand() %  (k_maxExplosionParticles - k_minExplosionParticles);
		
		float angleStep = (2.0f * _PI) / numParticles;
		float angle = unifRand(0.0f, _PI);
		
		m_positions.reserve(numParticles);
		m_directions.reserve(numParticles);
		
		for(int32 i = 0; i < numParticles; i++)
		{
			Vector3 point;
			
			float radius = unifRand(0.0f, k_initialExplosionRadius);
			point._x = radius * cos(angle);
			point._y = radius * sin(angle);

			m_positions.push_back((m_position + point));
			point.normalize();
			m_directions.push_back(point);

			angle+= angleStep;
		}
	}
	
	void Explosion::update(MILLISECONDS deltaTime)
	{
		float dt = deltaTime / 1000.0f;
		
		m_lifeTime+= dt;
		if(m_lifeTime >= k_explosionLifeTime)
		{
			EventPtr evt(new Event_Actor_Destroy(m_handle));
			TheEventMgr.pushEventToQueye(evt);
			return;
		}

		for(int32 i = 0; i < m_positions.size(); i++)
		{
			m_positions[i] = m_positions[i] + (m_directions[i] * k_particleVelocity * dt);	
		}
	}

	void Explosion::onDraw(GrafManager& graphManager)
	{
		int32 alpha = 255 - 255 * (m_lifeTime / k_explosionLifeTime);
		RGBCOLOR color = (alpha << 24) | 0x00ffffff;
		CURCOORD left, top;
		for(int32 i = 0; i < m_positions.size(); i++)
		{
			left = m_positions[i]._x - k_particleHalfSize;
			top = m_positions[i]._y - k_particleHalfSize;
			GrafManager::getInstance().drawEllipse(left, top, 
				k_particleSize, k_particleSize, color, 0xff000000); 
		}	
	}	
}