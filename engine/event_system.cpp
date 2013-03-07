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
				OSUtils::getInstance().debugOutput("add event listener [listener = 0x%x, event = %s]", listener, eventType.data());
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
					OSUtils::getInstance().debugOutput("remove event listener [listener = 0x%x, event = %s]", listener, eventType.data());
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
					OSUtils::getInstance().debugOutput("remove event listener [listener = 0x%x, event = %s]", listener, it->first.data());
				}
			}//for(EventListenerMap::iterator it = m_listeners.begin();
		}

		void EventManager::pushEventToQueye(EventPtr evt)
		{
			EventQueue& otherQueue = m_eventQueues[1- m_currentEventQueue];
			otherQueue.push(evt);

			OSUtils::getInstance().debugOutput("push event [event = %s]", evt->getType().data());
		}

		void EventManager::triggerEvent(EventPtr evt)
		{
			assert(evt != (long)0 && "invalid argument");

			OSUtils::getInstance().debugOutput("trigger event [event = %s]", evt->getType().data());

			EventListenerMap::iterator found_it = m_listeners.find(evt->getType());
			if(found_it == m_listeners.end()) 
				return;

			EventListenerSetPtr listenersSet = found_it->second;
			for(EventListenerSet::iterator it = listenersSet->begin();
					it != listenersSet->end(); ++it)
			{
				OSUtils::getInstance().debugOutput("process triggered event [event = %s, listener = 0x%x]", evt->getType().data(), (*it));
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
					OSUtils::getInstance().debugOutput("process pushed event [event = %s, listener = 0x%x]", evt->getType().data(), (*it));
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