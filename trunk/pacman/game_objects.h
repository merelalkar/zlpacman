#pragma once

#include "engine.h"
#include "tile_grid.h"

namespace Pegas
{
	enum Actors
	{
		k_actorAll = -1,
		k_actorPacman = 1,
		k_actorBlinky,
		k_actorPinky,
		k_actorInky,
		k_actorClyde
	};

	enum CollisionGroups
	{
		k_collisionGroupDefault = 0,
		k_collisionGroupDoor
	};

	enum Tiles
	{
		k_tilePill = 0,
		k_tileWall,
		k_tileSuperPill,
		k_tilePacman,
		k_tileDoor,
		k_tileTunnel,
		k_tileBlinky,
		k_tilePinky,
		k_tileInky,
		k_tileClyde,
		k_tileBonus
	};

	class Waiting: public Process
	{
	public:
		Waiting(float seconds, EventPtr	notifyEvent);
		virtual void update(MILLISECONDS deltaTime);		
	private:
		MILLISECONDS m_remainTime;
		EventPtr	 m_notifyEvent;
	};

	class Character: public IEventListener
	{
	public:
		enum eMovingDirections
		{
			k_moveLeft = 0,
			k_moveTop,
			k_moveRight,
			k_moveBottom
		};

	public:
		Character(int actorId);

		Vector3 getPosition() const { return m_position; }
		float	getRadius() const { return m_radius; }

		virtual void create(TileGrid* tileGrid, const Vector3& position);
		virtual void destroy();
		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);
		virtual void draw() {}

	protected:
		int		  m_actorId;	
		TileGrid* m_tileGrid;
		Vector3	  m_position;
		float	  m_radius;

		

		Vector3 m_directions[4];
		int		m_currentDirection;
		float	m_velocity;
		int		m_turnCommand;

		bool m_isBlocked;
		bool m_isMoving;
	};

	class Pacman: public Character 
	{
	public:
		enum Animation
		{
			k_animationRunning = 0,
			k_animationDeath,
			k_animationTotal
		};

	public:
		Pacman(int actorId, IPlatformContext* platform);

		virtual void create(TileGrid* tileGrid, const Vector3& position);
		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);
		virtual void draw();

		void setAnimation(int state, ProcessPtr animation);

	private:
		int32 m_prevRow;
		int32 m_prevColumn;

		IPlatformContext* m_platform;
		ProcessPtr	m_animations[k_animationTotal];
		int32		m_currentAnimation;
	};

	class Ghost: public Character 
	{
	public:
		virtual void handleEvent(EventPtr evt);
		virtual void update(float deltaTime);
	};
}