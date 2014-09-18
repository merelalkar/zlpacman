
#include "engine.h"
using namespace Pegas;

/******************************************************************
		ClickableGameObject class implementation
******************************************************************/

ClickableGameObject::ClickableGameObject()
	:m_pSucessor(0), m_zIndex(-1) 
{
	GUIManager& manager = GUIManager::getInstance();
	manager.addClickableObject(this);
}

ClickableGameObject::~ClickableGameObject() 
{
	GUIManager& manager = GUIManager::getInstance();
	manager.removeClickableObject(this);
}

void ClickableGameObject::click(CURCOORD x, CURCOORD y)
{
	if(m_pSucessor != 0)
		m_pSucessor->click(x, y);
}

bool ClickableGameObject::isIntersects(CURCOORD x, CURCOORD y)
{
	if(x < getLeft() || x > (getLeft() + getWidth()))
		return false;
	if(y < getTop() || y > (getTop() + getHeight()))
		return false;

	return true;
}

/**********************************************************************
		MovableGameObject class implementation
***********************************************************************/

void MovableGameObject::capture()
{
	GUIManager& guiManager = GUIManager::getInstance();
	guiManager.addMovableObject(this);	
}

void MovableGameObject::move(CURCOORD cx, CURCOORD cy)
{
	setLeft(getLeft() + cx);
	setTop(getTop() + cy);
}

void MovableGameObject::release(CURCOORD x, CURCOORD y)
{
	setLeft(x);
	setTop(y);

	GUIManager& guiManager = GUIManager::getInstance();
	guiManager.removeMovableObject(this);
}