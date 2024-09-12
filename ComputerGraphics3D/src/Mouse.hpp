#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <glm/glm.hpp>

#include "Const.hpp"

class Mouse
{
private:
	glm::vec2 m_Position;
	bool m_FirstMovement = true;
	
	static Mouse s_Mouse;

public:
	static Mouse& Get();
	
	glm::vec2 Move(float x, float y);


private:
	Mouse();
	~Mouse();
};

#endif // !MOUSE_HPP