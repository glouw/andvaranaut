#pragma once

#include "Display.h"

typedef struct
{
    Sdl sdl;
    Display display;
    int y;
}
Scanline;

void wrend(const Scanline scanline, const Ray ray);

void frend(const Scanline scanline, const Ray ray, const Map map, Point* const wheres, const Line fov);

void crend(const Scanline scanline, const Ray ray, const Map map, Point* const wheres);

void light(const Scanline scanline, const Ray ray, const Torch torch, Point* const wheres, int* const moddings);
