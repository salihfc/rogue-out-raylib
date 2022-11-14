#include "raylib.h"
#include "raymath.h"

#ifndef _UTILS
#define _UTILS

static const Rectangle SCREEN_BOUNDARY = (Rectangle) {.x = 0, .y = 0, .width = 1356, .height = 900};  

static
void ReverseRange(char* left, char* right)
{
    while (right > left)
    {
        {
            char tmp = *left;
            *left = *right;
            *right = tmp;
        }

        left++;
        right--;
    }
}


static 
const char* IntToConstChar(int number)
{
    const char* c = MemAlloc(10 * sizeof(char));
    char* it = (char*)c;

    if (number == 0)
    {
        *it = '0';
        return c;
    }

    if (number < 0)
    {
        *it = '-';
        it++;
        number = -number;
    }

    char* start = it;

    while (number > 0)
    {
        *it = '0' + (number % 10);

        it++;
        number /= 10;
    }

    ReverseRange(start, --it);

    // printf("%s\n", c);
    return c;
}

static 
const char* Vector2ToConstChar(Vector2 vec)
{
    const char* x_str = IntToConstChar((int)vec.x);
    const char* y_str = IntToConstChar((int)vec.y);

    const char* c = MemAlloc(64 * sizeof(char));
    char* it_c = (char*) c;

    char* it = (char*) x_str;
    while(*it != '\0')
    {
        *it_c = *it;

        it_c++;
        it++;
    }

    *it_c = ',';
    *it_c++;

    it = (char*) y_str;
    while(*it != '\0')
    {
        *it_c = *it;

        it_c++;
        it++;
    }

    MemFree((char*)x_str);
    MemFree((char*)y_str);
    return c;
}


static
bool InRange(float value, float start, float end)
{
    return (start <= value) && (value <= end);
}


static
float GetRandomFract()
{
    return GetRandomValue(0, __INT_MAX__) * 1.0f / __INT_MAX__;
}


static
float GetRandomFloat(float begin, float end)
{
    return begin + (end - begin) * GetRandomFract();
}


#endif