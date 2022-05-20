#include <iostream>

#include "Player.h"

Player::Player(float speed, float jump_force, int x, int y, int w, int h)
	: Object(x, y, w, h), m_speed{ speed }, is_grounded{ false }, m_velocity{ 0.0f, 0.0f }, m_jump_force{ jump_force } {}

Player::Player(float speed, float jump_force, const SDL_Rect& rect)
	: Object(rect), m_speed{ speed }, is_grounded{ false }, m_velocity{ 0.0f, 0.0f }, m_jump_force{ jump_force } {}

void Player::HandleMovement(float delta_time, std::span<Object> objects, const Bounds& level_bounds) {
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	m_velocity.x = 0.0f;
	if (keys[SDL_SCANCODE_LEFT]) m_velocity.x = -m_speed;
	if (keys[SDL_SCANCODE_RIGHT]) m_velocity.x = m_speed;

	if (is_grounded) {
		if (keys[SDL_SCANCODE_UP]) m_velocity.y = -m_jump_force;
		is_grounded = false;
	}

	MoveX(m_velocity.x * delta_time, objects, level_bounds);
	MoveY(m_velocity.y * delta_time, objects, level_bounds);
}

void Player::MoveX(float x, std::span<Object> objects, const Bounds& level_bounds) {
	real_x += x;
	m_hitbox.x = static_cast<int>(real_x);

	for (const Object& o : objects) {
		if (CollidesWith(o)) {
			if (x > 0.0f) { // Collision while moving right
				m_hitbox.x = o.GetHitbox().x - m_hitbox.w;
			}
			else { // Collision while moving left
				m_hitbox.x = o.GetHitbox().x + o.GetHitbox().w;
			}

			m_velocity.x = 0.0f;
		}
	}

	if (m_hitbox.x < level_bounds.left) m_hitbox.x = level_bounds.left;
	if (m_hitbox.x + m_hitbox.w > level_bounds.right) m_hitbox.x = level_bounds.right - m_hitbox.w;

	real_x = static_cast<float>(m_hitbox.x);
	real_y = static_cast<float>(m_hitbox.y);
}

void Player::MoveY(float y, std::span<Object> objects, const Bounds& level_bounds) {
	real_y += y;
	m_hitbox.y = static_cast<int>(real_y);

	for (const Object& o : objects) {
		if (CollidesWith(o)) {
			if (y > 0.0f) { // Collision while moving down
				m_hitbox.y = o.GetHitbox().y - m_hitbox.h;
				is_grounded = true;
				m_velocity.y = 0.0f;
			}
			else { // Collision while moving up
				m_hitbox.y = o.GetHitbox().y + o.GetHitbox().h;
				m_velocity.y = 0.0f;
			}
		}
	}

	if (m_hitbox.y < level_bounds.top) m_hitbox.y = level_bounds.top;
	if (m_hitbox.y + m_hitbox.h > level_bounds.bottom) m_hitbox.y = level_bounds.bottom - m_hitbox.h;

	real_x = static_cast<float>(m_hitbox.x);
	real_y = static_cast<float>(m_hitbox.y);
}

void Player::PrintVelocity() {
	std::cout << "Player X Velocity = " << m_velocity.x << '\n';
	std::cout << "Player Y Velocity = " << m_velocity.y << '\n';
}