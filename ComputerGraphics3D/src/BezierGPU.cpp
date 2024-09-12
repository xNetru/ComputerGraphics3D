#include "BezierGPU.hpp"

#include <vector>

BezierGPU::BezierGPU(const Shader& shader, const Texture2D& texture, glm::mat4 points, unsigned int resolution):
    m_Shader(shader), m_Texture(texture), m_Points(points), m_Resolution(resolution)
{
    
    std::vector<float> vertices;
    float step = 1.0f / (m_Resolution - 1);
    float x = 0.0f, y = 0.0f;
    for (int i = 0; i < m_Resolution - 1; i++)
    {
        x = 0.0f;
        for (int j = 0; j < m_Resolution - 1; j++)
        {
            // upper left triangle
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(x + step);
            vertices.push_back(y);
            vertices.push_back(x);
            vertices.push_back(y + step);
            // lower right triangle
            vertices.push_back(x + step);
            vertices.push_back(y);
            vertices.push_back(x);
            vertices.push_back(y + step);
            vertices.push_back(x + step);
            vertices.push_back(y + step);

            x += step;
        }
        y += step;
    }

    m_Step = step;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
}

BezierGPU::~BezierGPU()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

float& BezierGPU::At(int i, int j)
{
    return m_Points[i][j];
}

void BezierGPU::Draw(LightProperty& light, Camera& camera, Weather& weather, float deltaTime)
{
    glBindVertexArray(m_VAO);
    m_Shader.use();

    m_Texture.Bind();

    Object::configureIllumination(light, m_Shader);

    m_Shader.setMat4("controlPoints", m_Points);
 
    Move(deltaTime);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.1f, 1.5f, 2.4f));
    glm::mat4 view = camera.GetViewMatrix();

    Object::configureIlluminationVectors(light, m_Shader, view);

    m_Shader.setInt("shadingMode", weather.GetShadingMode());

    m_Shader.setVec3("viewPos", glm::vec3(0.0f));


    m_Shader.setMat4("model", model);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("projection", camera.GetProjectionMatrix());

    m_Shader.setFloat("step", m_Step);

    m_Shader.setBool("isDay", weather.IsDay());
    m_Shader.setFloat("fogDensity", weather.GetFogDensity());
    m_Shader.setVec3("skyColor", weather.GetSkyColour());

    m_Shader.setVec3("material.ambient", glm::vec3{ 0.1f,0.1f,0.1f });
    m_Shader.setFloat("material.shininess", 0.1f);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * (m_Resolution - 1) * (m_Resolution - 1));
    m_Texture.Unbind();
    glBindVertexArray(0);
}

void BezierGPU::Move(float deltaTime)
{
    m_Time += deltaTime;
    if (m_Time > 360.0f)
        m_Time -= 360.0f;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            At(i, j) = m_Amplitude * sin(m_WaveSpeed * glm::radians(i + j + m_Time));
        }
    }
}
