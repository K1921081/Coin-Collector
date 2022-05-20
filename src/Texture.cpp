#include <stdexcept>
#include <iostream>

#include "Texture.h"

Texture::Texture(const char* file_path, SDL_Renderer* renderer) {
	SDL_Surface* surface = SDL_LoadBMP(file_path);
	if (!surface) {
		std::cerr << "File: " << file_path << '\n';
		throw std::runtime_error("Could not load surface");
	}

	m_texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!m_texture) {
		std::cerr << "File: " << file_path << '\n';
		throw std::runtime_error("Could not create texture");
	}
}

Texture::Texture(const char* file_path, SDL_Renderer* renderer, Uint8 ck_r, Uint8 ck_g, Uint8 ck_b) {
	SDL_Surface* surface = SDL_LoadBMP(file_path);
	if (!surface) {
		std::cerr << "File: " << file_path << '\n';
		throw std::runtime_error("Could not load surface");
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, ck_r, ck_g, ck_b));
	m_texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!m_texture) {
		std::cerr << "File: " << file_path << '\n';
		throw std::runtime_error("Could not create texture");
	}
}

Texture::Texture(Texture&& t) noexcept {
	m_texture = t.m_texture;
	t.m_texture = nullptr;
}

Texture& Texture::operator=(Texture&& t) noexcept {
	if (m_texture) SDL_DestroyTexture(m_texture);
	m_texture = t.m_texture;
	t.m_texture = nullptr;
	return *this;
}

Texture::~Texture() {
	if (m_texture) SDL_DestroyTexture(m_texture);
}