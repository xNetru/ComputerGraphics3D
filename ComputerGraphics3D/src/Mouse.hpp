#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <glm/glm.hpp>

class Mouse
{
private:
	glm::dvec2 m_Position;
	static Mouse s_Mouse;

public:
	Mouse& get();
	
	const glm::dvec2& getPosition() const;
	void setPosition(const glm::dvec2& position);

private:
	Mouse();
	~Mouse();
};

#endif // !MOUSE_HPP