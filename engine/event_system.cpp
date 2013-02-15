#include "engine.h"
#include "event_system.h"

namespace Pegas
{
		void EventManager::addEventListener(EventListenerPtr listener, const EventType& eventType)
		{
			assert(listener != 0 && "invalid argument");

			EventListenerMap::iterator found_it = m_listeners.find(eventType);
			if(found_it == m_listeners.end())
			{
				EventListenerSetPtr eventListenersSet(new EventListenerSet());
				std::pair<EventListenerMap::iterator, bool> pair = m_listeners.insert(std::make_pair(eventType, eventListenersSet));
				found_it = pair.first;
			}

			if(found_it->second->count(listener) == 0)
			{
				found_it->second->insert(listener);
			}
		}

		void EventManager::removeEventListener(EventListenerPtr listener, const EventType& eventType)
		{
			assert(listener != (long)0 && "invalid argument");

			EventListenerMap::iterator found_it = m_listeners.find(eventType);
			if(found_it != m_listeners.end())
			{
				if(found_it->second->count(listener) > 0)
				{
					found_it->second->erase(listener);
				}
			}//if(found_it != m_listeners.end())			
		}

		void EventManager::removeEventListener(EventListenerPtr listener)
		{
			for(EventListenerMap::iterator it = m_listeners.begin();
				it != m_listeners.end(); ++it)
			{
				if(it->second->count(listener) > 0)
				{
					it->second->erase(listener);
				}
			}//for(EventListenerMap::iterator it = m_listeners.begin();
		}

		void EventManager::pushEventToQueye(EventPtr evt)
		{
			EventQueue& otherQueue = m_eventQueues[1- m_currentEventQueue];
			otherQueue.push(evt);
		}

		void EventManager::triggerEvent(EventPtr evt)
		{
			assert(evt != (long)0 && "invalid argument");

			EventListenerMap::iterator found_it = m_listeners.find(evt->getType());
			if(found_it == m_listeners.end()) 
				return;

			EventListenerSetPtr listenersSet = found_it->second;
			for(EventListenerSet::iterator it = listenersSet->begin();
					it != listenersSet->end(); ++it)
			{
				(*it)->handleEvent(evt);
			}
		}
			
		void EventManager::processEvents(MILLISECONDS timeLimit)
		{
			MILLISECONDS lastTime = OSUtils::getInstance().getCurrentTime();

			EventQueue& currentQueue = m_eventQueues[m_currentEventQueue];
			while(!currentQueue.empty())
			{
				EventPtr evt = currentQueue.front();
				currentQueue.pop();

				EventListenerMap::iterator found_it = m_listeners.find(evt->getType());
				if(found_it == m_listeners.end()) 
					continue;

				EventListenerSetPtr listenersSet = found_it->second;
				for(EventListenerSet::iterator it = listenersSet->begin();
					it != listenersSet->end(); ++it)
				{
					(*it)->handleEvent(evt);
				}
				
				if(timeLimit != kNoTimeLimit)
				{
					MILLISECONDS ellapsedTime = OSUtils::getInstance().getCurrentTime() - lastTime;
					if(ellapsedTime >= timeLimit)
						break;
				}
			}//while(!currentQueue.empty())
						
			if(currentQueue.empty())
			{
				m_currentEventQueue = 1 - m_currentEventQueue;
			}
		}
	
}//namespace pegas