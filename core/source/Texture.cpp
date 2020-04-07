#include "Texture.h"

#include <Utils.h>
#include <framework/Engine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture() {
	if (data) {
		stbi_image_free(data);
	}
}

unsigned char* Texture::getData() {
	return data;
}

int Texture::getWidth() {
	return m_width;
}

int Texture::getHeight() {
	return m_height;
}

void Texture::load(const char* path) {
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	std::string str_path = std::string(Engine::get()->getCorePath()) + path;
	data = stbi_load(str_path.c_str(), &m_width, &m_height, &m_channels, 0);
}