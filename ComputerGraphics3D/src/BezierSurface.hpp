#ifndef BEZIER_SURFACE_HPP
#define BEZIER_SURFACE_HPP

#include <memory>

#include <glm/glm.hpp>

#define BEZIER_SURFACE_DEGREE 3

class BezierSurface3x3 {
private:
	const unsigned int m_Resolution; // not lesser than 2

	float m_Binomial[BEZIER_SURFACE_DEGREE + 1] = { 1.0f, 3.0f, 3.0f, 1.0f };
	float m_XMonomialsBuffer[BEZIER_SURFACE_DEGREE + 1] = {};
	float m_YMonomialsBuffer[BEZIER_SURFACE_DEGREE + 1] = {};
	float m_ControlPoints[BEZIER_SURFACE_DEGREE + 1][BEZIER_SURFACE_DEGREE + 1] = { 0.0f };

	std::shared_ptr<float> m_Points;

public: 
	BezierSurface3x3(unsigned int resolution):
		m_Resolution{resolution}
	{
		m_Points = std::make_shared<float>(resolution * resolution);
	}

	struct BezierData {
		std::shared_ptr<glm::vec3> m_points;
	};

	float& ControlPointAt(int i, int j);
	std::shared_ptr<float> GetPoints();
	float GetZ(int x, int y);
};

#endif // !BEZIER_SURFACE_HPP
