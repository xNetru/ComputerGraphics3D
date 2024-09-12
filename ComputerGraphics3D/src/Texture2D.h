#pragma once

#include "Common.h"

#include <string>

class Texture2D
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BitPerPixel;
public:
	Texture2D(const std::string& filepath);
	~Texture2D();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetBitPerPixel() const { return m_BitPerPixel; }
};