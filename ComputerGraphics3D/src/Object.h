#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "Camera.hpp"
#include "Weather.h"
#include <glm/ext/matrix_float4x4.hpp>


enum Light_Movement {
    U,
    D,
    L,
    R
};

struct DirLight
{
    glm::vec3 direction = { -0.2f, -1.0f, -0.3f };
    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 diffuse = { 0.4f, 0.4f, 0.4f };
    glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
};
struct PointLight : DirLight
{
    glm::vec3 position = { -10.0f, -10.0f, 10.0f };
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
struct SpotLight : PointLight
{
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(15.0f));
    glm::vec3 initialPosition = { 10.0f, 10.0f, 10.0f };
    float yaw = -45.0f;
    float pitch = -45.0f;
};

struct LightProperty
{
    DirLight dirLight;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;

    const float lightSpeed = 50.0f;

    void processMovement(Light_Movement dir, float deltaTime)
    {
        for (int i = 0; i < spotLights.size(); i++)
        {
            if (dir == U)
                spotLights[i].pitch += deltaTime * lightSpeed;
            else if (dir == D)
                spotLights[i].pitch -= deltaTime * lightSpeed;
            else if (dir == R)
                spotLights[i].yaw += deltaTime * lightSpeed;
            else if (dir == L)
                spotLights[i].yaw -= deltaTime * lightSpeed;

            float pitch = spotLights[i].pitch; //+ glm::degrees(atan2(spotLights[i].direction.x, spotLights[i].direction.z));

            // glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), spotLights[i].direction));
            float yaw = spotLights[i].yaw; // + glm::degrees(atan2(-spotLights[i].direction.y, glm::length(right)));


            if (spotLights[i].pitch > 89.0f)
                spotLights[i].pitch = 89.0f;
            if (spotLights[i].pitch < -89.0f)
                spotLights[i].pitch = -89.0f;

            glm::vec3 newDir;
            newDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            newDir.y = sin(glm::radians(pitch));
            newDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            newDir = glm::normalize(newDir);
            spotLights[i].direction = newDir;
        }
    }
};

class Object
{
public:
    Shader& m_Shader;
    Model& m_Model;

    Object(Shader& shader, Model& model);
    static void configureIllumination(const LightProperty& light, const Shader& shader);
    static void configureIlluminationVectors(const LightProperty& prop, const Shader& shader, const glm::mat4& view);
    virtual void Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime) = 0;
};

class Cat : public Object
{
    glm::vec4 m_Position;
    float m_Time = 0.0f;
    float m_MovementVelocity = 20.0f;
    float m_RotationVelocity = 200.0f;

public:
    Cat(Shader& shader, Model& model, const glm::vec3& position);
    virtual void Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime) override;
};


class Board : public Object
{
    glm::vec3 m_Position;

public:
    Board(Shader& shader, Model& model, const glm::vec3& position);
    virtual void Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime) override;
};


class Sphere : public Object
{
    glm::vec3 m_Postion;
public:
    Sphere(Shader& shader, Model& model, const glm::vec3& position);
    virtual void Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime) override;
};