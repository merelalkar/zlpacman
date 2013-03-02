#pragma once

#include "engine.h"

namespace Pegas
{
	enum eGameEvents
	{
		k_eventChangeDirection = 0,
		k_eventCancelChangeDirection,
		k_eventDirectionChanged,
		k_eventCharacterStoped,
		k_eventCharacterMoveOn,
		k_eventEnableCharacterControl,
		k_eventDisableCharacterControl,
		k_eventPacmanSwallowedPill,
		k_eventPacmanDeath,
		k_eventPacmanDeathComplete
	};

	struct Event_ChangeDirection: public Event
	{
	public:
		Event_ChangeDirection(int actorId, int newDirection): 
		  _actorId(actorId), _newDirection(newDirection)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventChangeDirection;

		int _actorId;
		int _newDirection;
	};

	struct Event_CancelChangingDirection: public Event
	{
	public:
		Event_CancelChangingDirection(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventCancelChangeDirection;

		int _actorId;
	};

	struct Event_DirectionChanged: public Event
	{
	public:
		Event_DirectionChanged(int actorId, int newDirection): 
		  _actorId(actorId), _newDirection(newDirection)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventDirectionChanged;

		int _actorId;
		int _newDirection;
	};

	struct Event_CharacterStopped: public Event
	{
	public:
		Event_CharacterStopped(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventCharacterStoped;

		int _actorId;
	};

	struct Event_CharacterMoveOn: public Event
	{
	public:
		Event_CharacterMoveOn(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventCharacterMoveOn;

		int _actorId;
	};

	struct Event_EnableCharacterControl: public Event
	{
	public:
		Event_EnableCharacterControl(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventEnableCharacterControl;

		int _actorId;
	};

	struct Event_DisableCharacterControl: public Event
	{
	public:
		Event_DisableCharacterControl(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventDisableCharacterControl;

		int _actorId;
	};

	struct Event_PacmanSwallowedPill: public Event
	{
	public:
		Event_PacmanSwallowedPill(int pill): _pill(pill)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventPacmanSwallowedPill;

		int _pill;
	};

	struct Event_PacmanDeath: public Event
	{
	public:
		Event_PacmanDeath()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventPacmanDeath;		
	};

	struct Event_PacmanDeathComplete: public Event
	{
	public:
		Event_PacmanDeathComplete()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type = k_eventPacmanDeathComplete;		
	};
}