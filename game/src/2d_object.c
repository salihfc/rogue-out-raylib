#include "raylib.h"
#include "utils.c"
#include "geo_utils.c"

typedef
struct Object2D
{
    Vector2 position;
    Vector2 size;
    Vector2 velocity;

    float damping_factor;

} Object2D;

static
void ApplyForceToObject(Object2D* object, Vector2 force)
{
    object->velocity = VectorSum(object->velocity, force);
}


static
void Moveobject(Object2D* object, float delta)
{
    object->position = VectorSum(object->position, object->velocity);

    object->position.x = Clamp(object->position.x, 0, SCREEN_BOUNDARY.width  - object->size.x);
    object->position.y = Clamp(object->position.y, 0, SCREEN_BOUNDARY.height - object->size.y);

    // Damping
    object->velocity.x *= object->damping_factor;
    object->velocity.y *= object->damping_factor;
}

