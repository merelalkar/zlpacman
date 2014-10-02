// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#ifdef _WINDWOWS
#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>
#endif

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#ifdef _WINDOWS
#include <tchar.h>
#endif

// TODO. Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
#include "engine.h"
/*
#ifdef _WINDOWS
#include "platform_windows.h"
#endif
#ifdef ANDROID
#include "platform_android.h"
#endif*/
