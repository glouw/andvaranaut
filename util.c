#include "util.h"

#include <SDL2/SDL.h>

int fl(const float x)
{
    return (int) x - (x < (int) x);
}

int cl(const float x)
{
    return (int) x + (x > (int) x);
}

float dec(const float x)
{
    return x - (int) x;
}

int newlines(FILE* const fp)
{
    char* line = NULL;
    unsigned reads = 0;
    int lines = 0;
    while(getline(&line, &reads, fp) != -1) lines++;
    free(line);
    rewind(fp);
    return lines;
}

bool done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT
    || event.key.keysym.sym == SDLK_F1
    || event.key.keysym.sym == SDLK_ESCAPE)
        return true;
    return false;
}
