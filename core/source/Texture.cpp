#include "Texture.h"

#include <Utils.h>
#include <framework/Engine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>

Texture::Texture() {

}

Texture::Texture(const Texture& texture) {
	if (m_data) {
		stbi_image_free(m_data);
	}
	m_data = (unsigned char *) stbi__malloc(texture.m_data_size);
	memset(m_data, 0, sizeof(*m_data));
	memcpy(m_data, texture.m_data, texture.m_data_size);
	m_data_size = texture.m_data_size;
	m_width = texture.m_width;
	m_height = texture.m_height;
	m_channels = texture.m_channels;
}

Texture& Texture::operator=(Texture other) {
	if (m_data) {
		stbi_image_free(m_data);
	}
	m_data = (unsigned char*)stbi__malloc(other.m_data_size);
	memset(m_data, 0, sizeof(*m_data));
	memcpy(m_data, other.m_data, other.m_data_size);
	m_data_size = other.m_data_size;
	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;
	return *this;
}

Texture::~Texture() {
	if (m_data) {
		stbi_image_free(m_data);
	}
}

unsigned char* Texture::getData() {
	return m_data;
}

int Texture::getDataSize() {
	return m_data_size;
}

int Texture::getWidth() {
	return m_width;
}

int Texture::getHeight() {
	return m_height;
}

void Texture::load(const char* path) {
	std::filesystem::path fs_path(path);
	std::string str_path;
	if ((fs_path.is_absolute() && std::filesystem::exists(fs_path)) ||
		(fs_path.is_relative() && std::filesystem::exists((fs_path = std::string(Engine::get()->getAssetsPath()) + path)))) {
		str_path = fs_path.u8string();
	}

	if (!str_path.empty()) {
		stbi_set_flip_vertically_on_load(true);
		m_data = stbi_load(str_path.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
		m_data_size = m_width * m_height * (int)STBI_rgb_alpha;
	}
}