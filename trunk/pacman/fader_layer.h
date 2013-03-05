#pragma once
#include "default_game_state.h"

namespace Pegas
{
	class FaderLayer: public BaseScreenLayer, public IEventListener
	{
	public:
		FaderLayer(const LayerId& id);

		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);
		virtual void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
		virtual void render(IPlatformContext* context);
		virtual void handleEvent(EventPtr evt);

	private:
		RGBCOLOR		m_color;
		MILLISECONDS	m_fadeLength;
		MILLISECONDS	m_ellapsedTime;
		CURCOORD		m_width;
		CURCOORD		m_height;
		bool			m_fadein;	
	};
}