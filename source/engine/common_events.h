#pragma once

#include "interfaces.h"

namespace Pegas
{
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

	struct Event_GUI_FadeOn: public Event
	{
	public:
		Event_GUI_FadeOn(int32 fadePercent): _fadePercent(fadePercent) 
		{
			OSUtils::getInstance().debugOutput("Event_GUI_FadeOn [percent = %d]", fadePercent);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _fadePercent;
	};

	struct Event_GUI_FadeOff: public Event
	{
	public:
		Event_GUI_FadeOff() 
		{
			OSUtils::getInstance().debugOutput("Event_GUI_FadeOff");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
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

	struct Event_Game_Resume: public Event
	{
	public:
		Event_Game_Resume()  
		{
			OSUtils::getInstance().debugOutput("Event_Game_Resume");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};
}