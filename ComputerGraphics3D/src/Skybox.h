#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <string>

#include "Common.h"
#include "StbUtils.h"
#include "Camera.hpp"
#include "Shader.h"

class Skybox
{
private:
    unsigned int m_RendererID;
    unsigned int m_SkyboxVAOID, m_SkyboxVBOID;

    Shader& m_Shader;

public:
    Skybox(const std::vector<std::string>& faces, Shader& shader)
        : m_Shader(shader)
    {

        const float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &m_SkyboxVAOID);
        glGenBuffers(1, &m_SkyboxVBOID);
        glBindVertexArray(m_SkyboxVAOID);
        glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        m_RendererID = loadCubemap(faces);
    }
    ~Skybox()
    {
        glDeleteVertexArrays(1, &m_SkyboxVAOID);
        glDeleteBuffers(1, &m_SkyboxVBOID);
    }

public:

    void Bind(unsigned int slot)
    {
        glBindVertexArray(m_SkyboxVAOID);
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
    }
    void Unbind()
    {
        glBindVertexArray(0);
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    void Draw(const Camera& camera)
    {
        glDepthFunc(GL_LEQUAL);
        m_Shader.use();
        m_Shader.setMat4("projection", camera.GetProjectionMatrix());
        m_Shader.setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        Bind(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Unbind();
        glDepthFunc(GL_LESS); // set depth function back to default
    }
};