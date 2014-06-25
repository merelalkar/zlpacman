#pragma once

namespace Pegas
{
	/**********************************************************************
		Game actors
	******************************************************************/
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

	struct Event_Actor_Moved: public Event
	{
	public:
		Event_Actor_Moved(int32 actorID, const Vector3& position)
		{
			_actorID = actorID;
			_position = position;
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorID;
		Vector3 _position;
	};

	struct Event_Actor_SetPoisition: public Event
	{
	public:
		Event_Actor_SetPoisition(int32 actorID, const Vector3& position)
		{
			_actorID = actorID;
			_position = position;
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorID;
		Vector3 _position;
	};

	/************************************************************************************
		Player control
	************************************************************************************/
	struct Event_Player_EnableControl: public Event
	{
	public:
		Event_Player_EnableControl() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_DisableControl: public Event
	{
	public:
		Event_Player_DisableControl() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_RotateLeft: public Event
	{
	public:
		Event_Player_RotateLeft() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_RotateRight: public Event
	{
	public:
		Event_Player_RotateRight() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_Thrust: public Event
	{
	public:
		Event_Player_Thrust() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_Stop_Thrust: public Event
	{
	public:
		Event_Player_Stop_Thrust() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_Fire: public Event
	{
	public:
		Event_Player_Fire() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_Stop_Fire: public Event
	{
	public:
		Event_Player_Stop_Fire() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};

	struct Event_Player_Stop_Rotation: public Event
	{
	public:
		Event_Player_Stop_Rotation() { }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;			
	};
}