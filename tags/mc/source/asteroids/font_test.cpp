#include "stdafx.h"
#include "font_test.h"

namespace Pegas
{
	FontTest::FontTest()
		:DefaultGameState(1)
	{
	
	}

	void FontTest::enter(IPlatformContext* context)
	{

	}

	void FontTest::render(IPlatformContext* context)
	{
		GrafManager::getInstance().render();
	}
}