#ifndef OBJECT_H
#define OBJECT_H

#include <span>

#include "SDL.h"

#include "Texture.h"
#include "Physics.h"

// Basic game object
class Object {
public:
	Object(int x = 0, int y = 0, int w = 0, int h = 0);
	Object(const SDL_Rect& hitbox);

	// Render a texture at this object's hitbox
	void Render(SDL_Renderer* renderer, Texture& texture) {
		texture.Render(renderer, nullptr, &m_hitbox);
	}

	// Render a texture at the object's hitbox, and adjusted for the camera coords
	void Render(SDL_Renderer* renderer, Texture& texture, const SDL_Rect& camera);

	// Check if the coords (x, y) are inside the object's hitbox
	bool IsClicked(int x, int y) {
		return (x > m_hitbox.x && x < m_hitbox.x + m_hitbox.w &&
				y > m_hitbox.y && y < m_hitbox.y + m_hitbox.h);
	}

	// Set the location of the object
	void SetLocation(int x, int y) {
		m_hitbox.x = x;
		m_hitbox.y = y;
		real_x = static_cast<float>(x);
		real_y = static_cast<float>(y);
	}

	const SDL_Rect& GetHitbox() const { return m_hitbox; }

	/**
	 * @brief Sets the size of the object
	 * @param w New width
	 * @param h New height
	 * 
	 * @throws invalid_argument if w or h is below 0
	*/
	void SetSize(int w, int h);

	// Horizontal movement, with collision and bounds checking
	void MoveX(float x, std::span<Object> objects, const Bounds& level_bounds);

	// Vertical movement, with collision and bounds checking
	void MoveY(float y, std::span<Object> objects, const Bounds& level_bounds);

	/**
	 * @brief Checks if there's any collision with the other object
	 * @param object The object the collision checking is done with
	 * @return true if there's a collision, false otherwise
	*/
	bool CollidesWith(const Object& object);

	// Delete the object
	void Delete() { is_deleted = true; }

	/**
	 * @brief Checks if the object is deleted
	 * @return true if the object has been deleted, false otherwise
	*/
	const bool IsDeleted() const { return is_deleted; }

protected:
	SDL_Rect m_hitbox; // The object's hitbox

	/* Floating-point coords.They will be converted to int when rendering.
	   Used for framerate-independent movement */
	float real_x, real_y; 

	bool is_deleted; // Checks if the object is supposed to be deleted or not
};

/**
 * @brief Checks for collision between two objects
 * @param obj1 The first object
 * @param obj2 The second object
 * @return true if there's a collision, false otherwise
*/
bool AreColliding(const Object& obj1, const Object& obj2);

// Game object and a texture grouped together
struct ObjectAndTexture {
	Object object;
	Texture texture;
};

#endif