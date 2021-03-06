#ifndef AETNA_TEXTURE_H
#define AETNA_TEXTURE_H

class Texture {
private:
	unsigned char* m_data = nullptr;
	int m_data_size = 0;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;

public:
	Texture();
	Texture(const Texture& texture);
	Texture& operator=(Texture other);
	virtual ~Texture();

	unsigned char* getData();
	int getDataSize();
	int getWidth();
	int getHeight();
	void load(const char* path);
};

#endif //AETNA_TEXTURE_H