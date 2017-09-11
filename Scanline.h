#pragma once

#include "Display.h"

typedef struct
{
    Sdl sdl;
    Display display;
    int y;
}
Scanline;

void wrend(const Scanline scanline, const Ray ray, const Torch torch, int* const moddings);

void frend(const Scanline scanline, const Ray ray, const Map map, Point* const wheres,
    const Hero hero, int* const moddings);

void crend(const Scanline scanline, const Ray ray, const Map map, Point* const wheres);

void light(const Scanline scanline, int* const moddings);
