#include "raylib.h"

#ifndef _UTILS
#define _UTILS

static 
const char* IntToConstChar(int number)
{
    const char* c = MemAlloc(10 * sizeof(char));
    char* it = (char*)c;

    while (number > 0)
    {
        *it = '0' + (number % 10);

        it++;
        number /= 10;
    }

    return c;
}

#endif