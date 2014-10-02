// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#ifdef _WINDWOWS
#define WIN32_LEAN_AND_MEAN             // ��������� ����� ������������ ���������� �� ���������� Windows
// ����� ���������� Windows:
#include <windows.h>
#endif

// ����� ���������� C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#ifdef _WINDOWS
#include <tchar.h>
#endif

// TODO. ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
#include "engine.h"
/*
#ifdef _WINDOWS
#include "platform_windows.h"
#endif
#ifdef ANDROID
#include "platform_android.h"
#endif*/
