#pragma once

typedef struct
{
    int bot;
    int top;
}
Clamped;

Clamped c_clamp(const int yres, const float bot, const float top);
