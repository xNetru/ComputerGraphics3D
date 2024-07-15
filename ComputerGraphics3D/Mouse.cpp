#include "Mouse.hpp"

Mouse Mouse::s_Mouse;

Mouse& Mouse::get()
{
    return s_Mouse;
}

const glm::dvec2& Mouse::getPosition() const
{
    return m_Position;
}

void Mouse::setPosition(const glm::dvec2& position)
{
    m_Position = position;
}

Mouse::Mouse()
    : m_Position{ 0.0, 0.0 }
{
}

Mouse::~Mouse()
{
}
