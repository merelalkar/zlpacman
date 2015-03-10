#pragma once

namespace Pegas
{
	class FontTest: public DefaultGameState
	{
	public:
		FontTest();

		virtual void enter(IPlatformContext* context);
		virtual void render(IPlatformContext* context);
	};	
}