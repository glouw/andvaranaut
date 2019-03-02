#include "Theme.h"

const char* t_get_name(const Theme th)
{
    static const char* strings[] = {
#define X(a) #a,
        LIST_OF_THEMES
#undef X
    };
    return strings[th];
}
