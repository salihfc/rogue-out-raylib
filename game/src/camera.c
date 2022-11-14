#include "raylib.h"

#define MAX_SHAKE_ANGLE 5.0
#define MAX_OFFSET 10.0
#define TRAUMA_LOSS_PER_SECOND 1.0

typedef
struct CameraManager
{
	Camera2D camera;

	float trauma; // [0 - 1]

} CameraManager;


static
float GetShake(CameraManager* manager)
{
	return manager->trauma * manager->trauma;
}


static
void CameraAddTrauma(CameraManager* manager, float amount)
{
	manager->trauma = Clamp(manager->trauma + amount, 0.0, 1.0);
}


static
void TickCameraManager(CameraManager* manager, float delta)
{
	CameraAddTrauma(manager, -TRAUMA_LOSS_PER_SECOND * delta);
}


static
Camera2D GetModifiedCamera(CameraManager* manager)
{
	Camera2D camera = manager->camera;

	float shake = GetShake(manager);
	float shake_angle = MAX_SHAKE_ANGLE * shake * GetRandomFloat(-1, 1);
	float x_offset = MAX_OFFSET * shake * GetRandomFloat(-1, 1);
	float y_offset = MAX_OFFSET * shake * GetRandomFloat(-1, 1);

	camera.offset = VectorSum(camera.offset, (Vector2) {x_offset, y_offset});
	camera.rotation += shake_angle;

	return camera;
}