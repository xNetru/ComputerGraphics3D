#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include "Camera.hpp"
#include "Mouse.hpp"
#include "Weather.h"
#include "Object.h"
#include "BezierGPU.hpp"

class Scene
{
private:
	Scene();
	~Scene();

	static Scene s_Scene;

	GLFWwindow* window;

	// camera
	glm::vec3 m_DefaultPosition = glm::vec3(0.0f, 2.0f, 3.0f);
	const float m_DefaultPitch = -40.0f;
	const float m_DefaultYaw = -90.0f;

	const glm::vec3 spherePosition1 = glm::vec3(3.0f, 1.5f, -1.0f);
	const glm::vec3 spherePosition2 = glm::vec3(-3.0f, 1.0f, -2.0f);
	const glm::vec3 spotLightPosition1 = glm::vec3(0.0f, 10.0f, -5.0f);

	CameraType m_CameraType = CameraType::FREE;
	Camera m_Camera = Camera(m_DefaultPosition);

	// mouse
	Mouse& m_Mouse = Mouse::Get();

	//keyboard
	bool m_KeyWasPressed = false;
		
	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// weather
	Weather m_Weather;

	// light
	LightProperty m_Light;

public:
	static Scene& Get();
	void run();

	void ChangeCameraType();

	void configureLightProperty(LightProperty& prop);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window);
};

