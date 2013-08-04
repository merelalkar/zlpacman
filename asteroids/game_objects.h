#pragma once

namespace Pegas
{
	class Asteroid: public Process, public IEventListener
	{
	public:
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();
	};

	class Shatter: public Asteroid
	{
	public:
		virtual void handleEvent(EventPtr evt);
	};
	
	class Bullet: public Process, public IEventListener
	{
	public:
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();
	};

	class Ship: public Process, public IEventListener
	{
	public:
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();
	};

}