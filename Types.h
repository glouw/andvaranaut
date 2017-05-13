#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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
    SDL_Texture** texture;
}
Textures;

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
    char** block;
    Point column;
}
Range;

typedef struct
{
    Point* wheres;
    int* moddings;
}
Calc;

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

typedef enum
{
    N, E, S, W
}
Compass;

typedef struct
{
    int tile;
    float offset;
    Point where;
    Compass facing;
}
Hit;

typedef enum
{
    A, B,
    FRAMES
}
Frame;

typedef enum
{
    IDLE,
    STATES
}
State;

typedef struct
{
    Point where;
    int ascii;
    State state;
    bool transparent;
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
    float torch;
    float brightness;
    float dtorch;
}
Light;


typedef enum
{
    FLORING, WALLING, CEILING
}
Party;

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    float theta;
    Light light;
    int block;
    Party party;
    bool consoling;
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
    float* party;
    Light light;
}
Tracery;

typedef struct
{
    Traceline traceline;
    Wall wall;
    Hit hit;
}
Impact;

typedef struct
{
    int res;
    int fps;
    Surfaces surfaces;
    Textures textures;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int renders;
    int ticks;
    TTF_Font* font;
}
Sdl;

typedef struct
{
    uint32_t* pixels;
    int width;
}
Display;

typedef struct
{
    Sdl sdl;
    Display display;
    int y;
}
Scanline;

typedef struct
{
    Scanline scanline;
    Wall wall;
}
Boundary;

typedef struct
{
    char* text;
    SDL_Color color;
    int outline;
}
Ttf;
