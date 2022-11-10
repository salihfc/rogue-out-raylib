#include "raylib.h"

#ifndef _UTILS
#define _UTILS


static
void ReverseRange(char* left, char* right)
{
    while ((right != left+1) && (right != left))
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
float Clamp(float value, float mn, float mx)
{
    if (value >= mx)
        return mx;

    if (value <= mn)
        return mn;

    return value;
}


static
Vector2 VectorSum(Vector2 a, Vector2 b)
{
    return (Vector2) {
        a.x + b.x,
        a.y + b.y
    };
}

#endif