#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"

// Wrapper class for SDL_Texture
class Texture {
public:
	Texture() : m_texture{ nullptr } {}
	Texture(const char* file_path, SDL_Renderer* renderer);
	Texture(const char* file_path, SDL_Renderer* renderer, Uint8 ck_r, Uint8 ck_g, Uint8 ck_b);

	Texture(const Texture& t) = delete;
	Texture(Texture&& t) noexcept;
	Texture& operator=(const Texture& t) = delete;
	Texture& operator=(Texture&& t) noexcept;
	~Texture();

	// Get the underlying pointer to the SDL_Texture
	SDL_Texture* Get() const noexcept { return m_texture; }

	// Render the texture
	void Render(SDL_Renderer* renderer, const SDL_Rect* src, const SDL_Rect* dest) {
		SDL_RenderCopy(renderer, m_texture, src, dest);
	}

private:
	SDL_Texture* m_texture;
};

#endif