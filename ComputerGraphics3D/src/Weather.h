#pragma once

#pragma once
#include <glm/glm.hpp>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Weather
{
private:
	bool m_Fog = false;
	bool m_Day = true;

	int m_ShadingMode = 0;

public:

	bool IsDay() const
	{
		return m_Day;
	}

	bool IsFog() const
	{
		return m_Fog;
	}

	void ChangeTime()
	{
		m_Day = !m_Day;
	}

	void ChangeFog()
	{
		m_Fog = !m_Fog;
	}

	float GetFogDensity() const
	{
		return m_Fog ? 0.1f : 0.0f;
	}

	glm::vec3 GetSkyColour() const
	{
		return m_Day ? glm::vec3(0.53f, 0.81f, 0.98f) : glm::vec3(0.3f, 0.3f, 0.3f);
	}

	int GetShadingMode() const
	{
		return m_ShadingMode;
	}

	void ChangeShadingMode()
	{
		m_ShadingMode = (m_ShadingMode + 1) % 3;
	}
};