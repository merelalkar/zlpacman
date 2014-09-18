
#include "engine.h"
using namespace Pegas;


void GUIManager::mouseLButtonDown(CURCOORD x, CURCOORD y)
{
	IClickable* pObject = findClickableObject(x, y);

	if(pObject != NULL)
	{
		pObject->click(x, y);
	}
}

void GUIManager::mouseLButtonUp(CURCOORD x, CURCOORD y)
{
	for(MovableObjectList::iterator it = m_movableObjects.begin();
		it != m_movableObjects.end(); ++it)
	{
		(*it)->release(x, y);
	}

	m_movableObjects.clear();
}

void GUIManager::mouseMove(CURCOORD cx, CURCOORD cy)
{
	for(MovableObjectList::iterator it = m_movableObjects.begin();
		it != m_movableObjects.end(); ++it)
	{
		(*it)->move(cx, cy);		
	}
}

void GUIManager::addClickableObject(IClickable* pObject)
{
	if(m_clickableObjects.count(pObject) == 0)
	{
		m_clickableObjects.insert(pObject);		
	}	
}

void GUIManager::removeClickableObject(IClickable* pObject)
{
	m_clickableObjects.erase(pObject);		
}

void GUIManager::addMovableObject(IMovable* pObject)
{
	if(m_movableObjects.count(pObject) == 0)
	{
		m_movableObjects.insert(pObject);
	}
}

void GUIManager::removeMovableObject(IMovable* pObject)
{
	m_movableObjects.erase(pObject);	
}


IClickable* GUIManager::findClickableObject(CURCOORD x, CURCOORD y)
{
	std::vector<IClickable*> intersectedObjects;
	intersectedObjects.reserve(10);
	
	for(ClickableObjectList::iterator it = m_clickableObjects.begin();
		it != m_clickableObjects.end(); ++it)
	{
		IClickable* pObject = *it;
		
		if(pObject->isIntersects(x, y))
		{
			intersectedObjects.push_back(pObject);
		}
	}

	std::sort(intersectedObjects.begin(), 
			intersectedObjects.end(), clickableObjectsSortPredicate);
	
	if(intersectedObjects.size() > 0)
	{
		return intersectedObjects.front();
	}

	return NULL;
}

bool GUIManager::clickableObjectsSortPredicate(IClickable* a, IClickable* b)
{
	return a->getZIndex() > b->getZIndex();
}
