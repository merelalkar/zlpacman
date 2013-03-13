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
		Event_PacmanSwallowedPill(int pill): _pill(pill)  {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int _pill;
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

	/***************************************************************************************************
		GUI events;
	****************************************************************************************************/
	class Widget;
	struct Event_GUI_ButtonClick: public Event
	{
	public:
		Event_GUI_ButtonClick(Widget* button): m_button(button) {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Widget* m_button;
	};

	/*************************************************************************************************************
		Fader events;
	**************************************************************************************************************/
	struct Event_GUI_StartFadein: public Event
	{
	public:
		Event_GUI_StartFadein(float lengthInSeconds = 1.0f): 
		  _lengthInSeconds(lengthInSeconds) 
		  {
			  OSUtils::getInstance().debugOutput("Event_GUI_StartFadein [time = %.4f]", lengthInSeconds);
		  }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		float _lengthInSeconds;
	};

	struct Event_GUI_StartFadeout: public Event
	{
	public:
		Event_GUI_StartFadeout(float lengthInSeconds = 1.0f): 
		  _lengthInSeconds(lengthInSeconds)  
		  {
			  OSUtils::getInstance().debugOutput("Event_GUI_StartFadeout [time = %.4f]", lengthInSeconds);
		  }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		float _lengthInSeconds;
	};

	struct Event_GUI_FadeinComplete: public Event
	{
	public:
		Event_GUI_FadeinComplete()  
		{
			OSUtils::getInstance().debugOutput("Event_GUI_FadeinComplete");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_GUI_FadeoutComplete: public Event
	{
	public:
		Event_GUI_FadeoutComplete()  
		{
			OSUtils::getInstance().debugOutput("Event_GUI_FadeoutComplete");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	/*************************************************************************************************************
		Global Game events
	**************************************************************************************************************/
	struct Event_Game_ChangeState: public Event
	{
	public:
		Event_Game_ChangeState(const GameStateID& id): _id(id)  
		{
			OSUtils::getInstance().debugOutput("Event_Game_ChangeState [id = %d]", id);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		GameStateID _id;
	};

	struct Event_Game_ForwardToState: public Event
	{
	public:
		Event_Game_ForwardToState(const GameStateID& id): _id(id)  
		{
			OSUtils::getInstance().debugOutput("Event_Game_ForwardToState [id = %d]", id);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		GameStateID _id;
	};

	struct Event_Game_BackwardToPreviousState: public Event
	{
	public:
		Event_Game_BackwardToPreviousState() 
		{
			OSUtils::getInstance().debugOutput("Event_Game_BackwardToPreviousState");			
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_Game_Pause: public Event
	{
	public:
		Event_Game_Pause()  
		{
			OSUtils::getInstance().debugOutput("Event_Game_Pause");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};
}