#include "stdafx.h"
#include "gui_layer.h"

using namespace Pegas;

void GUILayer::addWidget(WidgetPtr widget)
{
	m_widgets.push_back(widget);	
}

void GUILayer::changeFocusNext()
{
	if(m_widgets.size() == 0) return;

	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->killFocus();
	}

	m_focusedWidget++;
	if(m_focusedWidget == m_widgets.end())
	{
		m_focusedWidget = m_widgets.begin();
	}

	(*m_focusedWidget)->setFocus();	
}

void GUILayer::changeFocusPrev()
{
	if(m_widgets.size() == 0) return;

	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->killFocus();
	}

	m_focusedWidget--;
	if(m_focusedWidget == m_widgets.end())
	{
		m_focusedWidget = --m_widgets.end();
	}

	(*m_focusedWidget)->setFocus();
}

void GUILayer::render(IPlatformContext* context)
{
	for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
	{
		(*it)->render(context);
	}
}

void GUILayer::create(IPlatformContext* context)
{
	m_focusedWidget = m_widgets.end();
}

void GUILayer::destroy(IPlatformContext* context)
{
	m_widgets.clear();
}

void GUILayer::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	if(isActive())
	{
		for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
		{
			if((*it) == (*m_focusedWidget)) continue;

			if((*it)->isPointIn(x, y))
			{
				(*it)->setFocus();
				(*it)->onMouseButtonDown(button, x, y, flags);

				if(m_focusedWidget != m_widgets.end())
				{
					(*m_focusedWidget)->killFocus();
				}

				m_focusedWidget = it;
				break;

			}//if((*it)->isPointIn(x, y))
		}//for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
	}//if(isActive())

	BaseScreenLayer::onMouseButtonDown(button, x, y, flags);
}

void GUILayer::onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags)
{
	if(isActive())
	{
		for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
		{
			if((*it) == (*m_focusedWidget)) continue;

			if((*it)->isPointIn(x, y))
			{
				(*it)->onMouseButtonUp(button, x, y, flags);
				break;
			}//if((*it)->isPointIn(x, y))
		}//for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
	}//if(isActive())

	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->onMouseButtonUp(button, x, y, flags);
	}

	BaseScreenLayer::onMouseButtonUp(button, x, y, flags);
}

void GUILayer::onMouseMove(float x, float y, MouseFlags flags)
{
	if(isActive())
	{
		for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
		{
			(*it)->onMouseMove(x, y, flags);
		}//for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
	}//if(isActive())

	BaseScreenLayer::onMouseMove(x, y, flags);
}

void GUILayer::onMouseWheel(NumNothes wheel, MouseFlags flags)
{
	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->onMouseWheel(wheel, flags);
	}

	BaseScreenLayer::onMouseWheel(wheel, flags);
}

void GUILayer::onKeyDown(KeyCode key, KeyFlags flags)
{
	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->onKeyDown(key, flags);
	}

	BaseScreenLayer::onKeyDown(key, flags);
}

void GUILayer::onKeyUp(KeyCode key, KeyFlags flags)
{
	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->onKeyUp(key, flags);
	}

	BaseScreenLayer::onKeyUp(key, flags);
}

void GUILayer::onChar(tchar ch)
{
	if(m_focusedWidget != m_widgets.end())
	{
		(*m_focusedWidget)->onChar(ch);
	}

	BaseScreenLayer::onChar(ch);
}