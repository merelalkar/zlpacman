#pragma once
#include "default_game_state.h"

namespace Pegas
{
	class Widget
	{
	public:
		virtual bool isPointIn(float x, float y) { return false; }
		virtual void setFocus() {}
		virtual void killFocus() {}

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags) {}
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags) {}
		virtual void onMouseMove(float x, float y, MouseFlags flags) {}
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags) {}

		virtual void onKeyDown(KeyCode key, KeyFlags flags) {}
		virtual void onKeyUp(KeyCode key, KeyFlags flags) {}
		virtual void onChar(tchar ch) {}

		virtual void render(IPlatformContext* context) {}
	};

	typedef SmartPointer<Widget> WidgetPtr;

	class GUILayer: public BaseScreenLayer
	{
	public:
		GUILayer(const LayerId& id, bool modal): 
		  BaseScreenLayer(_text("GUI"), id, modal) {}

		void addWidget(WidgetPtr widget);
		void changeFocusNext();
		void changeFocusPrev();

		virtual void render(IPlatformContext* context);
		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseMove(float x, float y, MouseFlags flags);
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void onChar(tchar ch);

	protected:
		typedef std::list<WidgetPtr> WidgetList;
		typedef WidgetList::iterator WidgetListIt;

		WidgetList		m_widgets;
		WidgetListIt	m_focusedWidget;
	};
}