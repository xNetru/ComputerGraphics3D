#include "BezierSurface.hpp"


float& BezierSurface3x3::ControlPointAt(int i, int j)
{
	return m_ControlPoints[i][j];
}

std::shared_ptr<float> BezierSurface3x3::GetPoints()
{
	float step = 1.0f / (m_Resolution - 1);
	float x = 0.0f;
	float y = 0.0f;
	for (int i = 0; i < m_Resolution; i++)
	{
		for (int j = 0; j < m_Resolution; j++)
		{
			m_Points.get()[i * m_Resolution + j] = GetZ(x, y);
			x += step;
		}
		y += step;
	}
	return m_Points;
}

float BezierSurface3x3::GetZ(int x, int y)
{
	float x1 = 1.0f - x;
	float y1 = 1.0f - y;
	float poweredX1 = x1;
	float poweredY1 = y1;

	m_XMonomialsBuffer[0] = 1.0f;
	m_YMonomialsBuffer[0] = 1.0f;
	for (int i = 1; i <= BEZIER_SURFACE_DEGREE; i++)
	{
		m_XMonomialsBuffer[i] = m_XMonomialsBuffer[i - 1] * x;
		m_YMonomialsBuffer[i] = m_YMonomialsBuffer[i - 1] * y;
	}
	for (int i = BEZIER_SURFACE_DEGREE - 1; i >= 0; i--)
	{
		m_XMonomialsBuffer[i] *= poweredX1;
		m_YMonomialsBuffer[i] *= poweredY1;
		poweredX1 *= x1;
		poweredY1 *= y1;
	}
	float z = 0.0f;
	for (int i = 0; i < BEZIER_SURFACE_DEGREE; i++)
	{
		for (int j = 0; j < BEZIER_SURFACE_DEGREE; j++) 
		{
			z += m_ControlPoints[i][j] * m_XMonomialsBuffer[j] * m_YMonomialsBuffer[i];
		}
	}
	return z;
}
