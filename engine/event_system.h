#pragma once

#include "singleton.h"
#include "smart_pointer.h"
#include "interfaces.h"

namespace Pegas
{
	
		class EventManager: public Singleton<EventManager> 
		{
		public:
            static const MILLISECONDS kNoTimeLimit = -1;

			EventManager():Singleton(*this), m_currentEventQueue(0) {};
			virtual ~EventManager() {};

			
			void addEventListener(EventListenerPtr listener, const EventType& eventType);
			void removeEventListener(EventListenerPtr listener, const EventType& eventType);
			void removeEventListener(EventListenerPtr listener);

			void pushEventToQueye(EventPtr evt);
			void triggerEvent(EventPtr evt);
			
			void processEvents(MILLISECONDS timeLimit = kNoTimeLimit);

		private:

			typedef std::queue<EventPtr> EventQueue;
			typedef std::set<EventListenerPtr> EventListenerSet;
			typedef SmartPointer<EventListenerSet> EventListenerSetPtr;
			typedef std::map<EventType, EventListenerSetPtr> EventListenerMap;

			EventQueue m_eventQueues[2];
			int m_currentEventQueue;
			
			EventListenerMap m_listeners;			

			EventManager(const EventManager& src);
			EventManager& operator=(const EventManager& src);
		};

		#define TheEventMgr EventManager::getInstance() 
		//#define TheEventMgrPtr EventManager::instancePtr()
	
}//namespace pegas