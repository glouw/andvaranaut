#include "Input.h"

#include "util.h"

#include <SDL2/SDL.h>

Input i_pump(Input input)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    if(input.key[SDL_SCANCODE_END]
    || input.key[SDL_SCANCODE_ESCAPE])
        input.done = true;

    const uint32_t buttons = SDL_GetRelativeMouseState(&input.dx, &input.dy);

    input.l = (buttons >> 0) & 0x1;
    input.m = (buttons >> 1) & 0x1;
    input.r = (buttons >> 2) & 0x1;

    input.lu = (input.l == 0 && input.ll == 1);
    input.mu = (input.m == 0 && input.lm == 1);
    input.ru = (input.r == 0 && input.lr == 1);

    input.ld = (input.l == 1 && input.ll == 0);
    input.md = (input.m == 1 && input.lm == 0);
    input.rd = (input.r == 1 && input.lr == 0);

    input.ll = input.l;
    input.lm = input.m;
    input.lr = input.r;

    SDL_GetMouseState(&input.x, &input.y);

    return input;
}

Input i_ready(const float sensitivity)
{
    static Input zero;
    Input input = zero;

    input.sx = sensitivity;
    input.sy = sensitivity;

    input.key = SDL_GetKeyboardState(NULL);

    return i_pump(input);
}

int i_using_inventory(const Input in)
{
    return in.key[SDL_SCANCODE_LSHIFT];
}

int i_using_world_edit_mode(const Input in)
{
    return in.key[SDL_SCANCODE_TAB];
}

int i_using_lookup(const Input in)
{
    return in.key[SDL_SCANCODE_BACKSPACE];
}

int i_get_alpha_key(const Input in)
{
    if(in.key[SDL_SCANCODE_A]) return 'A';
    if(in.key[SDL_SCANCODE_B]) return 'B';
    if(in.key[SDL_SCANCODE_C]) return 'C';
    if(in.key[SDL_SCANCODE_D]) return 'D';
    if(in.key[SDL_SCANCODE_E]) return 'E';
    if(in.key[SDL_SCANCODE_F]) return 'F';
    if(in.key[SDL_SCANCODE_G]) return 'G';
    if(in.key[SDL_SCANCODE_H]) return 'H';
    if(in.key[SDL_SCANCODE_I]) return 'I';
    if(in.key[SDL_SCANCODE_J]) return 'J';
    if(in.key[SDL_SCANCODE_K]) return 'K';
    if(in.key[SDL_SCANCODE_L]) return 'L';
    if(in.key[SDL_SCANCODE_M]) return 'M';
    if(in.key[SDL_SCANCODE_N]) return 'N';
    if(in.key[SDL_SCANCODE_O]) return 'O';
    if(in.key[SDL_SCANCODE_P]) return 'P';
    if(in.key[SDL_SCANCODE_Q]) return 'Q';
    if(in.key[SDL_SCANCODE_R]) return 'R';
    if(in.key[SDL_SCANCODE_S]) return 'S';
    if(in.key[SDL_SCANCODE_T]) return 'T';
    if(in.key[SDL_SCANCODE_U]) return 'U';
    if(in.key[SDL_SCANCODE_V]) return 'V';
    if(in.key[SDL_SCANCODE_W]) return 'W';
    if(in.key[SDL_SCANCODE_X]) return 'X';
    if(in.key[SDL_SCANCODE_Y]) return 'Y';
    if(in.key[SDL_SCANCODE_Z]) return 'Z';

    return EOF;
}

int i_get_numer_key(const Input in)
{
    if(in.key[SDL_SCANCODE_0]) return '0';
    if(in.key[SDL_SCANCODE_1]) return '1';
    if(in.key[SDL_SCANCODE_2]) return '2';
    if(in.key[SDL_SCANCODE_3]) return '3';
    if(in.key[SDL_SCANCODE_4]) return '4';
    if(in.key[SDL_SCANCODE_5]) return '5';
    if(in.key[SDL_SCANCODE_6]) return '6';
    if(in.key[SDL_SCANCODE_7]) return '7';
    if(in.key[SDL_SCANCODE_8]) return '8';
    if(in.key[SDL_SCANCODE_9]) return '9';

    return EOF;
}
