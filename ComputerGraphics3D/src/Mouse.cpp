#include "Mouse.hpp"

Mouse Mouse::s_Mouse = Mouse();

Mouse& Mouse::Get()
{
	return s_Mouse;
}

glm::vec2 Mouse::Move(float x, float y)
{
	if (m_FirstMovement)
	{
		m_FirstMovement = false;
		m_Position.x = x;
		m_Position.y = y;
		return { 0.0f, 0.0f };
	}

	float xoffset = x - m_Position.x;
	float yoffset = m_Position.y - y;

	m_Position.x = x;
	m_Position.y = y;
	return { xoffset, yoffset };
}

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}
