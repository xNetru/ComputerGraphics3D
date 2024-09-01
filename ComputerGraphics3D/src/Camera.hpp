#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
private:
	/* Position of the camera in the world space */
	glm::vec3 m_Position;

	/* Vectors that describe orientation in the model space */
	glm::vec3 m_RightUnitVector;
	glm::vec3 m_UpUnitVector;
	glm::vec3 m_FrontUnitVector;

public:
	Camera();
	
		
};

#endif // !CAMERA_HPP