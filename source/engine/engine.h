#ifndef PEGAS_ENGINE_H
#define PEGAS_ENGINE_H
#pragma once
/*
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <math.h>*/

//C Runtime
#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>
#include <assert.h>
#include <math.h>
/*
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cassert>
#include <stdexcept>
#include <stdarg.h>
*/

//STL

#include <exception>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <hash_map>


namespace Pegas
{
	class IDrawable;
	class IClickable;
	class IMovable;
	class GUIManager;
	class GrafManager;
	class SoundManager;
	class OSUtils;
}

#include "types.h"
#include "singleton.h"
#include "math_utils.h"
#include "vectors.h"
#include "matrix.h"
#include "resource_manager.h"
#include "utils.h"
#include "graf_manager.h"
#include "gui_manager.h"
#include "sound_manager.h"
#include "animation.h"
#include "game_objects_old.h"

#include "smart_pointer.h"
#include "interfaces.h"
#include "event_system.h"
#include "processes.h"

#include "common_events.h"
#include "default_game_state.h"
#include "gui_layer.h"
#include "fader_layer.h"
#include "standart_widgets.h"
#include "waiting.h"
#include "collision_checker.h"

#endif //PEGAS_ENGINE_H
