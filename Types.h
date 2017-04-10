#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct
{
    int rows;
    char** ceiling;
    char** walling;
    char** floring;
}
Map;

typedef struct
{
    int count;
    SDL_Surface** surface;
}
Surfaces;

typedef struct
{
    int bot;
    int top;
    int height;
    struct
    {
        int bot;
        int top;
    }
    clamped;
}
Wall;

typedef struct
{
    float x;
    float y;
}
Point;

typedef struct
{
    Point where;
    char* name;
}
Portal;

typedef struct
{
    int count;
    Portal* portal;
}
Portals;

typedef struct
{
    int tile;
    float offset;
    Point where;
}
Hit;

typedef struct
{
    Point where;
    int ascii;
}
Sprite;

typedef struct
{
    int count;
    Sprite* sprite;
}
Sprites;

typedef struct
{
    Point a;
    Point b;
}
Line;

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    struct
    {
        float theta;
        float percent;
    }
    angle;
}
Hero;

typedef struct
{
    Line trace;
    Point corrected;
    Line fov;
}
Traceline;

typedef struct
{
    Traceline traceline;
    Wall wall;
    Hit hit;
}
Impact;

typedef struct
{
    const int res;
    Surfaces surfaces;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
}
Gpu;

typedef struct
{
    uint32_t* pixels;
    int width;
}
Display;

typedef struct
{
    Gpu gpu;
    Display display;
    int y;
}
Scanline;
