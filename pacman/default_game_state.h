#pragma once
#include "engine.h"

namespace Pegas
{
	class BaseScreenLayer: public IScreenLayer, public IKeyboardController, public IMouseController
	{
	public:
		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags) = 0;
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags) = 0;
		virtual void onMouseMove(float x, float y, MouseFlags flags) = 0;
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags) = 0;

	protected:
		bool m_modal;
	};
}
