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
#include <conio.h>
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
#include "vectors.h"
#include "resource_manager.h"
#include "utils.h"
#include "graf_manager.h"
#include "gui_manager.h"
#include "sound_manager.h"
#include "animation.h"
#include "game_objects.h"

#include "smart_pointer.h"
#include "interfaces.h"
#include "event_system.h"
#include "processes.h"

#endif //PEGAS_ENGINE_H