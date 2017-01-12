#pragma once

#include "Hero.h"

const double Display_focal;
const int Display_xres;
const int Display_yres;

void
Display_Shutdown();

void
Display_RenderFrame(const Hero hero);

void
Display_Boot();
