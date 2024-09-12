#include "Texture2D.h"

#include "vendor/stb_image.h"

Texture2D::Texture2D(const std::string& filepath)
	: m_RendererID{ 0 }, m_FilePath{ filepath }, m_LocalBuffer{ nullptr },
	m_Width{ 0 }, m_Height{ 0 }, m_BitPerPixel{ 0 }
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BitPerPixel, 0);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLenum format;
	if (m_BitPerPixel == 1)
		format = GL_RED;
	else if (m_BitPerPixel == 3)
		format = GL_RGB;
	else if (m_BitPerPixel == 4)
		format = GL_RGBA;

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture2D::~Texture2D()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture2D::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
