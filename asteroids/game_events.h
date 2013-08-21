#pragma once

namespace Pegas
{
	struct Event_Actor_CreateAsteroid: public Event
	{
	public:
		Event_Actor_CreateAsteroid(const Vector3& position, const Vector3& direction)
		{
			_position = position;
			_direction = direction;
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Vector3 _position;
		Vector3 _direction;
	};

	struct Event_Actor_CreateShatter: public Event
	{
	public:
		Event_Actor_CreateShatter(const Vector3& position, const Vector3& direction)
		{
			_position = position;
			_direction = direction;
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Vector3 _position;
		Vector3 _direction;
	};

	struct Event_Actor_CreateBullet: public Event
	{
	public:
		Event_Actor_CreateBullet(const Vector3& position, const Vector3& direction)
		{
			_position = position;
			_direction = direction;
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Vector3 _position;
		Vector3 _direction;
	};

	struct Event_Actor_CreateExplosion: public Event
	{
	public:
		Event_Actor_CreateExplosion(const Vector3& position)
		{
			_position = position;			
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Vector3 _position;		
	};

	struct Event_Actor_Destroy: public Event
	{
	public:
		Event_Actor_Destroy(int32 actorID)
		{
			_actorID = actorID;			
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorID;		
	};
}