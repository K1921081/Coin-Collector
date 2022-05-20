#include "Camera.h"

void CenterScreenOnObject(const Object& object, SDL_Rect& camera, const Bounds& level_bounds) {
	camera.x = object.GetHitbox().x - object.GetHitbox().w / 2 - camera.w / 2;
	if (camera.x < level_bounds.left) camera.x = level_bounds.left;
	if (camera.x > level_bounds.right - camera.w) camera.x = level_bounds.right - camera.w;

	camera.y = object.GetHitbox().y - object.GetHitbox().h / 2 - camera.h / 2;
	if (camera.y < level_bounds.top) camera.y = level_bounds.top;
	if (camera.y > level_bounds.bottom - camera.h) camera.y = level_bounds.bottom - camera.h;
}