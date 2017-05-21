#include "Console.h"

static int shifting(const uint8_t* const key)
{
    const bool shift =
        key[SDL_SCANCODE_LSHIFT] ||
        key[SDL_SCANCODE_RSHIFT];
    if(shift && key[SDL_SCANCODE_1])
        return '!';
    if(shift && key[SDL_SCANCODE_2])
        return '@';
    if(shift && key[SDL_SCANCODE_3])
        return '#';
    if(shift && key[SDL_SCANCODE_4])
        return '$';
    if(shift && key[SDL_SCANCODE_5])
        return '%';
    if(shift && key[SDL_SCANCODE_6])
        return '^';
    if(shift && key[SDL_SCANCODE_7])
        return '&';
    if(shift && key[SDL_SCANCODE_8])
        return '*';
    if(shift && key[SDL_SCANCODE_9])
        return '(';
    if(shift && key[SDL_SCANCODE_0])
        return ')';
    if(shift && key[SDL_SCANCODE_A])
        return 'A';
    if(shift && key[SDL_SCANCODE_B])
        return 'B';
    if(shift && key[SDL_SCANCODE_C])
        return 'C';
    if(shift && key[SDL_SCANCODE_D])
        return 'D';
    if(shift && key[SDL_SCANCODE_E])
        return 'E';
    if(shift && key[SDL_SCANCODE_F])
        return 'F';
    if(shift && key[SDL_SCANCODE_G])
        return 'G';
    if(shift && key[SDL_SCANCODE_H])
        return 'H';
    if(shift && key[SDL_SCANCODE_I])
        return 'I';
    if(shift && key[SDL_SCANCODE_J])
        return 'J';
    if(shift && key[SDL_SCANCODE_K])
        return 'K';
    if(shift && key[SDL_SCANCODE_L])
        return 'L';
    if(shift && key[SDL_SCANCODE_M])
        return 'M';
    if(shift && key[SDL_SCANCODE_N])
        return 'N';
    if(shift && key[SDL_SCANCODE_O])
        return 'O';
    if(shift && key[SDL_SCANCODE_P])
        return 'P';
    if(shift && key[SDL_SCANCODE_Q])
        return 'Q';
    if(shift && key[SDL_SCANCODE_R])
        return 'R';
    if(shift && key[SDL_SCANCODE_S])
        return 'S';
    if(shift && key[SDL_SCANCODE_T])
        return 'T';
    if(shift && key[SDL_SCANCODE_U])
        return 'U';
    if(shift && key[SDL_SCANCODE_V])
        return 'V';
    if(shift && key[SDL_SCANCODE_W])
        return 'W';
    if(shift && key[SDL_SCANCODE_X])
        return 'X';
    if(shift && key[SDL_SCANCODE_Y])
        return 'Y';
    if(shift && key[SDL_SCANCODE_Z])
        return 'Z';
    if(shift && key[SDL_SCANCODE_EQUALS])
        return '+';
    if(shift && key[SDL_SCANCODE_SEMICOLON])
        return ':';
    if(shift && key[SDL_SCANCODE_COMMA])
        return '<';
    if(shift && key[SDL_SCANCODE_PERIOD])
        return '>';
    if(shift && key[SDL_SCANCODE_SLASH])
        return '?';
    if(shift && key[SDL_SCANCODE_APOSTROPHE])
        return '"';
    if(shift && key[SDL_SCANCODE_MINUS])
        return '_';
    if(shift && key[SDL_SCANCODE_GRAVE])
        return '~';
    if(shift && key[SDL_SCANCODE_RIGHTBRACKET])
        return '{';
    if(shift && key[SDL_SCANCODE_BACKSLASH])
        return '|';
    if(shift && key[SDL_SCANCODE_LEFTBRACKET])
        return '}';
    // Not pressed
    return -1;
}

static int mapping(const uint8_t* const key)
{
    if(key[SDL_SCANCODE_0])
        return '0';
    if(key[SDL_SCANCODE_1])
        return '1';
    if(key[SDL_SCANCODE_2])
        return '2';
    if(key[SDL_SCANCODE_3])
        return '3';
    if(key[SDL_SCANCODE_4])
        return '4';
    if(key[SDL_SCANCODE_5])
        return '5';
    if(key[SDL_SCANCODE_6])
        return '6';
    if(key[SDL_SCANCODE_7])
        return '7';
    if(key[SDL_SCANCODE_8])
        return '8';
    if(key[SDL_SCANCODE_9])
        return '9';
    if(key[SDL_SCANCODE_A])
        return 'a';
    if(key[SDL_SCANCODE_B])
        return 'b';
    if(key[SDL_SCANCODE_C])
        return 'c';
    if(key[SDL_SCANCODE_D])
        return 'd';
    if(key[SDL_SCANCODE_E])
        return 'e';
    if(key[SDL_SCANCODE_F])
        return 'f';
    if(key[SDL_SCANCODE_G])
        return 'g';
    if(key[SDL_SCANCODE_H])
        return 'h';
    if(key[SDL_SCANCODE_I])
        return 'i';
    if(key[SDL_SCANCODE_J])
        return 'j';
    if(key[SDL_SCANCODE_K])
        return 'k';
    if(key[SDL_SCANCODE_L])
        return 'l';
    if(key[SDL_SCANCODE_M])
        return 'm';
    if(key[SDL_SCANCODE_N])
        return 'n';
    if(key[SDL_SCANCODE_O])
        return 'o';
    if(key[SDL_SCANCODE_P])
        return 'p';
    if(key[SDL_SCANCODE_Q])
        return 'q';
    if(key[SDL_SCANCODE_R])
        return 'r';
    if(key[SDL_SCANCODE_S])
        return 's';
    if(key[SDL_SCANCODE_T])
        return 't';
    if(key[SDL_SCANCODE_U])
        return 'u';
    if(key[SDL_SCANCODE_V])
        return 'v';
    if(key[SDL_SCANCODE_W])
        return 'w';
    if(key[SDL_SCANCODE_X])
        return 'x';
    if(key[SDL_SCANCODE_Y])
        return 'y';
    if(key[SDL_SCANCODE_Z])
        return 'z';
    if(key[SDL_SCANCODE_EQUALS])
        return '=';
    if(key[SDL_SCANCODE_SEMICOLON])
        return ';';
    if(key[SDL_SCANCODE_COMMA])
        return ',';
    if(key[SDL_SCANCODE_PERIOD])
        return '.';
    if(key[SDL_SCANCODE_SLASH])
        return '/';
    if(key[SDL_SCANCODE_APOSTROPHE])
        return '\'';
    if(key[SDL_SCANCODE_MINUS])
        return '-';
    if(key[SDL_SCANCODE_GRAVE])
        return '`';
    if(key[SDL_SCANCODE_RIGHTBRACKET])
        return '[';
    if(key[SDL_SCANCODE_BACKSLASH])
        return '\\';
    if(key[SDL_SCANCODE_LEFTBRACKET])
        return ']';
    if(key[SDL_SCANCODE_SPACE])
        return ' ';
    // Not pressed
    return -1;
}

extern int lookup(const uint8_t* const key)
{
    const int shifts = shifting(key);
    if(shifts != -1)
        return shifts;
    const int mapped = mapping(key);
    if(mapped != -1)
        return mapped;
    // Not pressed
    return -1;
}
