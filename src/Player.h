#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Camera.h"
#include "Physics.h"

class Player : public Object {
public:
	Player(float speed, float jump_force, int x = 0, int y = 0, int w = 0, int h = 0);
	Player(float speed, float jump_force, const SDL_Rect& r);

	void HandleMovement(float delta_time, std::span<Object> objects, const Bounds& level_bounds);

	/**
	 * @brief Applies gravity to the player by constants reducing his y velocity
	 * @param g Surface gravity
	 * @param delta_time The time passed since the last frame
	*/
	void ApplyGravity(float g, float delta_time) { if (!is_grounded) m_velocity.y += g * delta_time; }

	// Prints the velocity of the player. Used for debugging
	void PrintVelocity();

	// Horizontal movement. Does bounds and collision checking
	void MoveX(float x, std::span<Object> objects, const Bounds& level_bounds);

	// Vertical movement. Does bounds and collision checking
	void MoveY(float y, std::span<Object> objects, const Bounds& level_bounds);

private:
	float m_speed;
	bool is_grounded;
	Vec2F m_velocity;
	float m_jump_force;
};

#endif