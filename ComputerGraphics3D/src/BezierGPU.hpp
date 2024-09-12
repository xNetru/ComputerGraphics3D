#ifndef BEZIER_GPU_HPP
#define BEZIER_GPU_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Shader.h"
#include "Camera.hpp"
#include "Weather.h"
#include "Object.h"
#include "Texture2D.h"

class BezierGPU
{
private:
	unsigned int m_VAO, m_VBO;

	unsigned int m_Resolution;

	float m_Step = 0.0f;

	glm::mat4 m_Points;
	Shader m_Shader;

	Texture2D m_Texture;

	float m_Time = 0.0f;
	float m_WaveSpeed = 10.0f;
	float m_Amplitude = 0.3f;
public:
	BezierGPU(const Shader& shader, const Texture2D& texture, glm::mat4 points = glm::mat4(0.0f), unsigned int resolution = 4);
	~BezierGPU();
	float& At(int i, int j);
	void Draw(LightProperty& light, Camera& camera, Weather& weather, float deltatime);
private:
	void Move(float deltaTime);
};

#endif
