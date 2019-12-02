﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "include/GL/glew.h"
#include "include/GL/glut.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/GL/freeglut.h"
#include <string>
#include <vector>
#include <map>
#include <mmsystem.h>

#pragma comment(lib, "winmm")
//#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/freeglut.lib")

#define UWM_CUSTOM1 WM_USER + 1
#define UWM_CHECKED WM_USER + 2

#endif PCH_H