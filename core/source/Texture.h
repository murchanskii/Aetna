#ifndef AETNA_TEXTURE_H
#define AETNA_TEXTURE_H

class Texture {
private:
	unsigned char* data = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;

public:
	virtual ~Texture();

	unsigned char* getData();
	int getWidth();
	int getHeight();
	void load(const char* path);
};

#endif //AETNA_TEXTURE_H