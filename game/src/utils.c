#include "raylib.h"

#ifndef _UTILS
#define _UTILS


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
void DrawInt(int number, Vector2 position, int fontsize, Color color)
{
    const char* p_text = IntToConstChar(number);
    DrawText(p_text, position.x, position.y, fontsize, color);
    MemFree((void *)p_text);
}


static
float Clamp(float value, float mn, float mx)
{
    if (value > mx)
        return mx;

    if (value < mn)
        return mn;

    return value;
}


// -------------
// VECTOR2 UTILS
// -------------

static
Vector2 VectorSum(Vector2 a, Vector2 b)
{
    return (Vector2) {
        a.x + b.x,
        a.y + b.y
    };
}

static
Vector2 VectorScaled(Vector2 v, float scale)
{
    return (Vector2) {
        v.x * scale,
        v.y * scale
    };
}


// -------------
// ~VECTOR2
// -------------


#endif