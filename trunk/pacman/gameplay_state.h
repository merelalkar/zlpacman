#pragma once

#include "engine.h"

namespace Pegas
{
		class GamePlayState: public Pegas::IGameState, public Pegas::IKeyboardController
		{
		public:
			virtual void enter(IPlatformContext* context);
			virtual void leave(IPlatformContext* context);
			virtual void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
			virtual void render(IPlatformContext* context);

			virtual void onKeyDown(KeyCode key, KeyFlags flags);
			virtual void onKeyUp(KeyCode key, KeyFlags flags);
			virtual void onChar(tchar ch);
		};
}