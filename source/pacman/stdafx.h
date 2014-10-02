// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // ��������� ����� ������������ ���������� �� ���������� Windows
// ����� ���������� Windows:
#include <windows.h>
#endif

// ����� ���������� C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#ifdef _WIN32
#include <tchar.h>
#endif

// TODO. ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
#include "engine.h"

#ifdef _WIN32
#include "platform_windows.h"
#endif
#ifdef ANDROID
#include "platform_android.h"
#endif
