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

void frend(const Scanline scanline, const Ray ray, Point* const wheres, char** const floring);

void crend(const Scanline scanline, const Ray ray, Point* const wheres, char** const ceiling);

void light(const Scanline scanline, const Ray ray, Point* const wheres, const Torch torch, int* const moddings);
