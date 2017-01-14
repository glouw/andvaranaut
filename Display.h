#pragma once

#include "Hero.h"

void
Display_RenderFrame(const Hero hero, const Map map);

void
Display_String(const char* string, const int x, const int y);

void
Display_ShowFPS(const int fps, const int x, const int y);

void
Display_Update();

void
Display_Shutdown();

void
Display_Boot();
