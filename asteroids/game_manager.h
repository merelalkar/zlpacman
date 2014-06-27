#pragma once

#include "game_objects.h"

namespace Pegas
{
	class GameManager: public BaseScreenLayer, public IEventListener
	{
	public:
		GameManager();

		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);
		virtual void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
		virtual void render(IPlatformContext* context);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		
		virtual void handleEvent(EventPtr evt);
		
		virtual ListenerType getListenerName() { return "GameManager"; }

	private:
		ProcessManager m_processes;
		CollisionManager m_collisionManager;
		std::map<ProcessHandle, IGameObject*> m_gameObjects;

		void setupWorldBound();
		void createShip();
		void createAsteroids();

		bool isObjectVisible(const Vector3& position);
		Vector3 getPositionToTeleport(const Vector3& position);

		Vector3 m_worldCenter;
		float m_worldLeft;
		float m_worldRight;
		float m_worldTop;
		float m_worldBottom;
	};	
}