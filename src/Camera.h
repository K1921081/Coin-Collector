#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

#include "Object.h"
#include "Physics.h"

/**
 * @brief Centers the screen around the object. Limited to the level bounds
 * 
 * @param object The object to center the screen around
 * @param camera The camera rect
 * @param level_bounds The level bounds
*/
void CenterScreenOnObject(const Object& object, SDL_Rect& camera, const Bounds& level_bounds);

#endif