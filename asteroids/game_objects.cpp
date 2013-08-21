#include "stdafx.h"
#include "game_objects.h"

#include "game_events.h"

namespace Pegas
{
	const int32 k_asteroidNumPoints = 7;
	const int32 k_shatterNumPoints = 5;

	const float k_asteroidVelosity = 1.0f;
	const float k_bulletVelocity = 3.0f;
	const float k_shipVelocity = 2.0f;

	const float k_asteroidRadius = 10.0f;
	const float k_shatterRadius = 5.0f;

	const int32 k_asteroidCollisionGroup = 1;
	const int32 k_shipCollisionGroup = 2;
	const int32 k_bulletCollisionGroup = 3;
	const int32	k_borderCollisionGroup = 4;

	
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
			m_points[i]+= offset;
		}
	}

	void Asteroid::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		generatePoints(k_asteroidNumPoints, k_asteroidRadius, m_points);
		movePoints(m_position);

		m_collisionManager.registerCircle(myHandle, k_asteroidCollisionGroup, m_position, k_asteroidRadius);
	}

	void Asteroid::update(MILLISECONDS deltaTime)
	{
		Vector3 offset = m_direction * k_asteroidVelosity * (deltaTime / 1000.0f);
		m_position+= offset;

		movePoints(offset);
		m_collisionManager.moveObject(m_handle, offset);
	}

	void Asteroid::terminate()
	{
		m_collisionManager.unregisterCollisionHull(m_handle);
	
		Process::terminate();
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
		
		for(int32 i = 0; i < numPoints; i++)
		{
			Vector3 point, direction;
			
			float radius = unifRand(0.0f, maxRadius);
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
	void Shatter::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		generatePoints(k_shatterNumPoints, k_asteroidRadius, k_shatterRadius);
		movePoints(m_position);

		m_collisionManager.registerCircle(myHandle, k_asteroidCollisionGroup, m_position, k_shatterRadius);
	}

	void Shatter::onCollisionEnter(IGameObject* other)
	{
		if(other->getType() == "Bullet")
		{
			EventPtr evt(new Event_Actor_CreateExplosion(m_position));
			TheEventMgr.pushEventToQueye(evt);

			EventPtr evt(new Event_Actor_Destroy(m_handle));
			TheEventMgr.pushEventToQueye(evt);
		}
	}
}