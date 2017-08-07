#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// All structs fields are natuarlly alligned
// through the use of ints, pointers, and floats

typedef struct
{
    // Clang like to break these pointers
    char** volatile ceiling;
    char** volatile walling;
    char** volatile floring;
    int rows;
}
Map;

typedef struct
{
    SDL_Texture** texture;
    int count;
}
Textures;

typedef struct
{
    SDL_Surface** surface;
    int count;
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

typedef enum
{
    E, SE, S, SW, W, NW, N, NE, DIRS
}
Compass;

typedef struct
{
    int surface;
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
    // Hurt directions
    H_E,
    H_SE,
    H_S,
    H_SW,
    H_W,
    H_NW,
    H_N,
    H_NE,
    // States
    IDLE,
    GRABBED,
    MERCY,
    STATES
}
State;

typedef struct
{
    Point where;
    int ascii;
    State state;
    int transparent;
    float width;
    float health;
    int ticks;
}
Sprite;

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
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
    float light;
    float brightness;
    float dtorch;
}
Torch;

typedef enum
{
    FLORING, WALLING, CEILING
}
Party;

typedef struct
{
    Point vect;
    union
    {
        int swing;
    }
    type;
    float power;
    Point where;
    float reach;
    float area;
}
Attack;

typedef enum
{
    LSWORD,
    WEAPONS
}
Weapon;

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    float theta;
    Torch torch;
    int surface;
    Party party;
    int inserting;
    int consoling;
    int saved;
    float arm;
    int floor;
    Weapon weapon;
    Attack attack;
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
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    TTF_Font* font;
    int res;
    int fps;
    Surfaces surfaces;
    Textures textures;
    int renders;
    int ticks;
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
    Impact impact;
}
Sliver;

typedef struct
{
    // Keyboard state
    const uint8_t* key;
    // Mouse sensitivity
    float sx;
    float sy;
    // Mouse deltas
    int dx;
    int dy;
    // Mouse Left, middle, right button state
    int l;
    int m;
    int r;
}
Input;
