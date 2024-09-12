#include "Object.h"

Object::Object(Shader& shader, Model& model)
	: m_Shader(shader), m_Model(model)
{
    
}

void Object::configureIllumination(const LightProperty& prop, const Shader& shader)
{
    // m_Shader.setVec3("dirLight.direction", prop.dirLight.direction);
    shader.setVec3("dirLight.ambient", prop.dirLight.ambient);
    shader.setVec3("dirLight.diffuse", prop.dirLight.diffuse);
    shader.setVec3("dirLight.specular", prop.dirLight.specular);
    // pointLight
    for (int i = 0; i < prop.pointLights.size(); i++)
    {
        // m_Shader.setVec3("pointLights[" + to_string(i) + "].position", prop.pointLights[i].position);
        shader.setVec3("pointLights[" + to_string(i) + "].ambient", prop.pointLights[i].ambient);
        shader.setVec3("pointLights[" + to_string(i) + "].diffuse", prop.pointLights[i].diffuse);
        shader.setVec3("pointLights[" + to_string(i) + "].specular", prop.pointLights[i].specular);
        shader.setFloat("pointLights[" + to_string(i) + "].constant", prop.pointLights[i].constant);
        shader.setFloat("pointLights[" + to_string(i) + "].linear", prop.pointLights[i].linear);
        shader.setFloat("pointLights[" + to_string(i) + "].quadratic", prop.pointLights[i].quadratic);
    }

    for (int i = 0; i < prop.spotLights.size(); i++)
    {
        // m_Shader.setVec3("spotLights[" + to_string(i) + "].position", prop.spotLights[i].position);
        // m_Shader.setVec3("spotLights[" + to_string(i) + "].direction", prop.spotLights[i].direction);
        shader.setVec3("spotLights[" + to_string(i) + "].ambient", prop.spotLights[i].ambient);
        shader.setVec3("spotLights[" + to_string(i) + "].diffuse", prop.spotLights[i].diffuse);
        shader.setVec3("spotLights[" + to_string(i) + "].specular", prop.spotLights[i].specular);
        shader.setFloat("spotLights[" + to_string(i) + "].constant", prop.spotLights[i].constant);
        shader.setFloat("spotLights[" + to_string(i) + "].linear", prop.spotLights[i].linear);
        shader.setFloat("spotLights[" + to_string(i) + "].quadratic", prop.spotLights[i].quadratic);
        shader.setFloat("spotLights[" + to_string(i) + "].cutOff", prop.spotLights[i].cutOff);
        shader.setFloat("spotLights[" + to_string(i) + "].outerCutOff", prop.spotLights[i].outerCutOff);
    }
}

void Object::configureIlluminationVectors(const LightProperty& prop, const Shader& shader, const glm::mat4& view)
{
    shader.setVec3("dirLight.direction", glm::vec3(view * glm::vec4(prop.dirLight.direction, 0.0f)));

    for (int i = 0; i < prop.pointLights.size(); i++)
    {
        shader.setVec3("pointLights[" + to_string(i) + "].position", glm::vec3(view * glm::vec4(prop.pointLights[i].position, 1.0f)));
    }
    for (int i = 0; i < prop.spotLights.size(); i++)
    {
        shader.setVec3("spotLights[" + to_string(i) + "].position", glm::vec3(view * glm::vec4(prop.spotLights[i].position, 1.0f)));
        shader.setVec3("spotLights[" + to_string(i) + "].direction", glm::vec3(view * glm::vec4(prop.spotLights[i].direction, 0.0f)));
    }

}

Cat::Cat(Shader& shader, Model& model, const glm::vec3& position)
    : Object(shader, model), m_Position(glm::vec4(position, 1.0f))
{
    
}


void Cat::Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime)
{
    m_Shader.use();
    configureIllumination(prop, m_Shader);

    // fog
    // day/night
    m_Time += deltaTime;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_MovementVelocity * m_Time), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(m_Position));
    model = glm::rotate(model, glm::radians(m_RotationVelocity * m_Time), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::vec3 pos = glm::vec3(model[3][0], model[3][1] + 0.4, model[3][2]);
    glm::vec3 dir = glm::vec3(-model[1][0], model[1][1], -model[1][2]);

    if (camera.Type == CameraType::MOVINGOBJECT)
    {
        camera.SetNewPosition(pos, dir);
    }

    if (camera.Type == CameraType::FOLLOWING)
    {
        camera.SetNewPosition(camera.Position, glm::vec3(model[3][0], model[3][1], model[3][2]) - camera.Position);
    }
    
    prop.pointLights[0].position = pos;
    prop.pointLights[1].position = pos;
    prop.pointLights[0].direction = glm::normalize(dir);
    prop.pointLights[1].direction = -glm::normalize(dir);
    prop.spotLights[0].position = pos;
    prop.spotLights[1].position = pos;
    prop.spotLights[1].direction = -prop.spotLights[0].direction;

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    configureIlluminationVectors(prop, m_Shader, view);

    m_Shader.setVec3("viewPos", glm::vec3(1.0f));

    m_Shader.setInt("shadingMode", weather.GetShadingMode());

    m_Shader.setMat4("model", model);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("projection", projection);

    m_Shader.setBool("isDay", weather.IsDay());
    m_Shader.setFloat("fogDensity", weather.GetFogDensity());
    m_Shader.setVec3("skyColor", weather.GetSkyColour());

    m_Shader.setVec3("material.ambient", glm::vec3{ 0.1f,0.1f,0.1f });
    m_Shader.setFloat("material.shininess", 0.1f);

    m_Model.Draw(m_Shader);
}

Board::Board(Shader& shader, Model& model, const glm::vec3& position)
    : Object(shader, model), m_Position(position)
{

}

void Board::Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime)
{
    m_Shader.use();

    // light

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(0.1f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    m_Shader.setMat4("model", model);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("projection", projection);

    m_Model.Draw(m_Shader);

    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f));
    model = glm::translate(model, glm::vec3(0.0f, 20.0f, -23.0f));
    m_Shader.setMat4("model", model);

    m_Model.Draw(m_Shader);
}

Sphere::Sphere(Shader& shader, Model& model, const glm::vec3& position)
    : Object(shader, model), m_Postion(position)
{
}

void Sphere::Draw(LightProperty& prop, Camera& camera, const Weather& weather, float deltaTime)
{
    m_Shader.use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, m_Postion);
    model = glm::scale(model, glm::vec3(0.1f));

    m_Shader.setMat4("model", model);
    m_Shader.setMat4("view", camera.GetViewMatrix());
    m_Shader.setMat4("projection", camera.GetProjectionMatrix());

    m_Model.Draw(m_Shader);
}
