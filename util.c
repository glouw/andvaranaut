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
    int lines = 0, pc = '\n', ch;
    while((ch = getc(fp)) != EOF)
    {
        if(ch == '\n')
            lines++;
        pc = ch;
    }
    if(pc != '\n')
        lines++;
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

// Requires unit testing
static char* readln(FILE* const fp)
{
    int size = 2;
    char* line = malloc(size * sizeof(char));
    int reads = 0;
    char ch;
    do
    {
        ch = getc(fp);
        line[reads++] = ch;
        if(reads + 1 == size)
            line = realloc(line, size *= 2);
    }
    while(ch != '\n' || ch != EOF);
    line[reads] = '\0';
    return line;
}
