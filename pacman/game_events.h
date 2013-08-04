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
		k_eventPacmanDeathComplete,

		k_eventGUI_ButtonClick,
		k_eventGUI_StartFadein,
		k_eventGUI_StartFadeout,
		k_eventGUI_FadeinComplete,
		k_eventGUI_FadeoutComplete,

		k_eventGame_ChangeState,
		k_eventGame_ForwardToState,
		k_eventGame_BackwardToPreviousState
	};

	struct Event_ChangeDirection: public Event
	{
	public:
		Event_ChangeDirection(int actorId, int newDirection): 
		  _actorId(actorId), _newDirection(newDirection)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
		int _newDirection;
	};

	struct Event_CancelChangingDirection: public Event
	{
	public:
		Event_CancelChangingDirection(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_DirectionChanged: public Event
	{
	public:
		Event_DirectionChanged(int actorId, int newDirection): 
		  _actorId(actorId), _newDirection(newDirection)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
		int _newDirection;
	};

	struct Event_CharacterStopped: public Event
	{
	public:
		Event_CharacterStopped(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_CharacterMoveOn: public Event
	{
	public:
		Event_CharacterMoveOn(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_CharacterMoved: public Event
	{
	public:
		Event_CharacterMoved(int32 actorId, const Vector3& position, int32 row, int32 column): 
		  _actorId(actorId), _position(position), _row(row), _column(column)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Vector3 _position;
		int32 _actorId;
		int32 _row;
		int32 _column;
	};

	struct Event_CharacterStateChanged: public Event
	{
	public:
		Event_CharacterStateChanged(int32 actorId, int32 newState): 
		  _actorId(actorId), _newState(newState) {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorId;
		int32 _newState;		
	};

	struct Event_CharacterChangeState: public Event
	{
	public:
		Event_CharacterChangeState(int32 actorId, int32 newState): 
		  _actorId(actorId), _newState(newState) {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorId;
		int32 _newState;		
	};

	struct Event_ShowCharacter: public Event
	{
	public:
		Event_ShowCharacter(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_HideCharacter: public Event
	{
	public:
		Event_HideCharacter(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_EnableCharacterControl: public Event
	{
	public:
		Event_EnableCharacterControl(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_DisableCharacterControl: public Event
	{
	public:
		Event_DisableCharacterControl(int actorId): _actorId(actorId)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
	};

	struct Event_PacmanSwallowedPill: public Event
	{
	public:
		Event_PacmanSwallowedPill(int pill, int row, int column): 
		  _pill(pill), _row(row), _column(column)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _pill;
		int _row;
		int _column;
	};

	struct Event_CharacterTonnelIn: public Event
	{
	public:
		Event_CharacterTonnelIn(int32 actorId, int32 row, int32 column):
		  _actorId(actorId), _row(row), _column(column) {}
			

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorId;
		int32 _row;
		int32 _column;
	};

	struct Event_CharacterTonnelOut: public Event
	{
	public:
		Event_CharacterTonnelOut(int32 actorId, int32 row, int32 column):
		  _actorId(actorId), _row(row), _column(column) {}
			

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _actorId;
		int32 _row;
		int32 _column;
	};

	struct Event_CharacterKilled: public Event
	{
	public:
		Event_CharacterKilled(int actorId, const Vector3& position): 
		  _actorId(actorId), _position(position)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _actorId;
		Vector3 _position;
	};

	struct Event_PacmanDeath: public Event
	{
	public:
		Event_PacmanDeath()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_PacmanDeathComplete: public Event
	{
	public:
		Event_PacmanDeathComplete()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_SuperForceOn: public Event
	{
	public:
		Event_SuperForceOn()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_SuperForceOff: public Event
	{
	public:
		Event_SuperForceOff()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_SuperForcePreOff: public Event
	{
	public:
		Event_SuperForcePreOff()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_BonusOn: public Event
	{
	public:
		Event_BonusOn(int32 row, int32 column): _row(row), _column(column) {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _row;
		int32 _column;
	};

	struct Event_BonusOff: public Event
	{
	public:
		Event_BonusOff()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_ResetActors: public Event
	{
	public:
		Event_ResetActors()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_RestartGame: public Event
	{
	public:
		Event_RestartGame()  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	/***************************************************************************************************
		HUD events
	****************************************************************************************************/
	struct Event_HUD_LevelChanged: public Event
	{
	public:
		Event_HUD_LevelChanged(int32 level): _level(level)  
		{
			OSUtils::getInstance().debugOutput("Event_HUD_LevelChanged [level = %d]", level);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _level;
	};

	struct Event_HUD_NewLevel: public Event
	{
	public:
		Event_HUD_NewLevel(int32 level, float showTimeSeconds): 
		  _level(level), _showTimeSeconds(showTimeSeconds)  
		{
			OSUtils::getInstance().debugOutput("Event_HUD_NewLevel [level = %d, time = %.2f]", level, showTimeSeconds);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _level;
		float _showTimeSeconds;
	};

	struct Event_HUD_ScoresChanged: public Event
	{
	public:
		Event_HUD_ScoresChanged(int32 scores): _scores(scores)  
		{
			OSUtils::getInstance().debugOutput("Event_HUD_ScoresChanged [scores = %d]", scores);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _scores;
	};

	struct Event_HUD_LivesChanged: public Event
	{
	public:
		Event_HUD_LivesChanged(int32 lives): _lives(lives)  
		{
			OSUtils::getInstance().debugOutput("Event_HUD_LivesChanged [lives = %d]", lives);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _lives;
	};

	struct Event_HUD_Frag: public Event
	{
	public:
		Event_HUD_Frag(int32 scores, const Vector3& position): 
		  _scores(scores), _position(position)   
		{
			OSUtils::getInstance().debugOutput("Event_HUD_Frag [scores = %d, x = %.2f, y = %.2f]", 
				scores, position._x, position._y);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32	_scores;
		Vector3 _position;
	};

	struct Event_HUD_GetReady: public Event
	{
	public:
		Event_HUD_GetReady(bool show = true): _show(show)  
		{
			OSUtils::getInstance().debugOutput("Event_HUD_GetReady [show = %d]", show ? 1 : 0);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		bool _show;
	};

	struct Event_HUD_GameOver: public Event
	{
	public:
		Event_HUD_GameOver(bool show = true): _show(show)  
		{
			OSUtils::getInstance().debugOutput("Event_HUD_GameOver [show = %d]", show ? 1 : 0);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		bool _show;
	};	
}