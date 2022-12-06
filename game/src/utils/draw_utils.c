#include "utils.c"

static
void DrawInt(int number, Vector2 position, int fontsize, Color color)
{
    const char* p_text = IntToConstChar(number);
    DrawText(p_text, position.x, position.y, fontsize, color);
    MemFree((void *)p_text);
}

static
void DrawVector2(Vector2 vec, Vector2 position, int fontsize, Color color)
{
    const char* text = Vector2ToConstChar(vec);
    DrawText(text, position.x, position.y, fontsize, color);
    MemFree((void*)text);
}