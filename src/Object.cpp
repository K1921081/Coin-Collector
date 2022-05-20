#include <stdexcept>

#include "Object.h"

Object::Object(int x, int y, int w, int h)
	: m_hitbox{ x, y, w, h }, real_x{ static_cast<float>(x) }, real_y{ static_cast<float>(y) }, is_deleted{ false } {}

Object::Object(const SDL_Rect& rect) : m_hitbox{ rect }, real_x{ static_cast<float>(rect.x) },
	real_y{ static_cast<float>(rect.y) }, is_deleted{ false } {}

void Object::Render(SDL_Renderer* renderer, Texture& texture, const SDL_Rect& camera) {
	SDL_Rect rendering_area{ m_hitbox.x - camera.x, m_hitbox.y - camera.y, m_hitbox.w, m_hitbox.h };
	texture.Render(renderer, nullptr, &rendering_area);
}

void Object::MoveX(float x, std::span<Object> objects, const Bounds& level_bounds) {
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
		}
	}

	if (m_hitbox.x < level_bounds.left) m_hitbox.x = level_bounds.left;
	if (m_hitbox.x + m_hitbox.w > level_bounds.right) m_hitbox.x = level_bounds.right - m_hitbox.w;

	real_x = static_cast<float>(m_hitbox.x);
	real_y = static_cast<float>(m_hitbox.y);
}

void Object::MoveY(float y, std::span<Object> objects, const Bounds& level_bounds) {
	real_y += y;
	m_hitbox.y = static_cast<int>(real_y);

	for (const Object& o : objects) {
		if (CollidesWith(o)) {
			if (y > 0.0f) { // Collision while moving down
				m_hitbox.y = o.GetHitbox().y - m_hitbox.h;
			}
			else { // Collision while moving up
				m_hitbox.y = o.GetHitbox().y + o.GetHitbox().h;
			}
		}
	}

	if (m_hitbox.y < level_bounds.top) m_hitbox.y = level_bounds.top;
	if (m_hitbox.y + m_hitbox.h > level_bounds.bottom) m_hitbox.y = level_bounds.bottom - m_hitbox.h;

	real_x = static_cast<float>(m_hitbox.x);
	real_y = static_cast<float>(m_hitbox.y);
}

bool Object::CollidesWith(const Object& object) {
	SDL_Rect hb = object.GetHitbox();

	if (m_hitbox.x > hb.x - m_hitbox.w && m_hitbox.x < hb.x + hb.w &&
		m_hitbox.y > hb.y - m_hitbox.h && m_hitbox.y < hb.y + hb.h)
		return true;
	return false;
}

void Object::SetSize(int w, int h) {
	if (w < 0 || h < 0) throw std::invalid_argument("Invalid object size: width or height below 0");

	m_hitbox.w = w;
	m_hitbox.h = h;
}

bool AreColliding(const Object& obj1, const Object& obj2) {
	SDL_Rect hb1 = obj1.GetHitbox();
	SDL_Rect hb2 = obj2.GetHitbox();

	if (hb1.x > hb2.x - hb1.w && hb1.x < hb2.x + hb2.w &&
		hb1.y > hb2.y - hb1.h && hb1.y < hb2.y + hb2.h)
		return true;
	return false;
}