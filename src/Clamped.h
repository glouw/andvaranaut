#pragma once

typedef struct
{
    int bot;
    int top;
}
Clamped;

Clamped xzclamp();

Clamped xclamp(const int yres, const float bot, const float top);
