#include "Scene.h"
#include <iostream>
#include "Const.hpp"
#include "Mouse.hpp"
#include "Shader.h"
#include "Skybox.h"
#include "Object.h"

#include <filesystem>

namespace fs = std::filesystem;

static std::string getAbsolutePath(const std::string& relativePath) {
    fs::path relative(relativePath);
    fs::path absolute = fs::absolute(relative);
    return absolute.string();
}

Scene Scene::s_Scene;

void framebufferSizeCallbackHandle(GLFWwindow* window, int width, int height)
{
    Scene& scene = Scene::Get();
    scene.framebuffer_size_callback(window, width, height);
}

void mouseCallbackHandle(GLFWwindow* window, double xposIn, double yposIn)
{
    Scene& scene = Scene::Get();
    scene.mouse_callback(window, xposIn, yposIn);
}

void scrollCallbackHandle(GLFWwindow* window, double xoffset, double yoffset)
{
    Scene& scene = Scene::Get();
    scene.scroll_callback(window, xoffset, yoffset);
}

void Scene::configureLightProperty(LightProperty& prop)
{
    prop.dirLight.direction = { -0.2f, -1.0f, -0.3f };
    prop.dirLight.ambient = { 0.5f, 0.5f, 0.5f };
    prop.dirLight.diffuse = { 0.4f, 0.4f, 0.4f };
    prop.dirLight.specular = { 0.5f, 0.5f, 0.5f };

    PointLight p1;
    p1.position = spherePosition1;
    p1.ambient = { 0.1f, 0.1f, 0.1f };
    p1.diffuse = { 0.3f, 0.3f, 0.5f };
    p1.specular = { 0.1f, 0.1f, 0.1f };
    p1.constant = 1.0f;
    p1.linear = 0.09f;
    p1.quadratic = 0.032f;
    prop.pointLights.push_back(p1);

    PointLight p2;
    p2.position = spherePosition2;
    p2.ambient = { 0.1f, 0.1f, 0.1f };
    p2.diffuse = { 0.3f, 0.3f, 0.5f };
    p2.specular = { 0.1f, 0.1f, 0.1f };
    p2.constant = 1.0f;
    p2.linear = 0.09f;
    p2.quadratic = 0.032f;
    prop.pointLights.push_back(p2);

    SpotLight s1;
    s1.position = spotLightPosition1;
    s1.initialPosition = spotLightPosition1;
    s1.pitch = 0.0f;
    s1.yaw = 0.0f;
    s1.ambient = { 0.1f, 0.1f, 0.1f };
    s1.diffuse = { 1.0f, 1.0f, 1.0f };
    s1.specular = { 1.0f, 1.0f, 1.0f };
    s1.constant = 0.1f;
    s1.linear = 0.01f;
    s1.quadratic = 0.001f;
    s1.cutOff = glm::cos(glm::radians(10.0f));
    s1.outerCutOff = glm::cos(glm::radians(20.0f));
    prop.spotLights.push_back(s1);

    SpotLight s2;
    s2.position = spotLightPosition1;
    s2.initialPosition = spotLightPosition1;
    s2.pitch = 0.0f;
    s2.yaw = 0.0f;
    s2.ambient = { 0.1f, 0.1f, 0.1f };
    s2.diffuse = { 1.0f, 1.0f, 1.0f };
    s2.specular = { 1.0f, 1.0f, 1.0f };
    s2.constant = 0.1f;
    s2.linear = 0.01f;
    s2.quadratic = 0.001f;
    s2.cutOff = glm::cos(glm::radians(10.0f));
    s2.outerCutOff = glm::cos(glm::radians(20.0f));
    prop.spotLights.push_back(s2);

    prop.processMovement(U, 0);
}

Scene::Scene()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackHandle);
    glfwSetCursorPosCallback(window, mouseCallbackHandle);
    glfwSetScrollCallback(window, scrollCallbackHandle);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    m_Camera.SetNewPosition(m_DefaultPosition, m_DefaultPitch, m_DefaultYaw);
    m_Camera.SetCameraType(m_CameraType);

    configureLightProperty(m_Light);
}

void Scene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Scene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    glm::vec2 offset = m_Mouse.Move(xpos, ypos);

    m_Camera.ProcessMouseMovement(offset.x, offset.y);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Scene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Scene::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !m_KeyWasPressed)
    {
        ChangeCameraType();
        m_KeyWasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !m_KeyWasPressed)
    {
        m_Weather.ChangeTime();
        m_KeyWasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !m_KeyWasPressed)
    {
        m_Weather.ChangeFog();
        m_KeyWasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !m_KeyWasPressed)
    {
        m_Weather.ChangeShadingMode();
        m_KeyWasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        m_KeyWasPressed = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        m_Light.processMovement(U, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_Light.processMovement(L, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_Light.processMovement(R, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_Light.processMovement(D, deltaTime);
}

Scene::~Scene()
{
    glfwTerminate();
}

Scene& Scene::Get()
{
	return s_Scene;
}

void Scene::run()
{
    Shader skyboxShader("res/shaders/skybox.vertex.shader", "res/shaders/skybox.fragment.shader");
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    
    std::vector<std::string> dayFaces
    {
        getAbsolutePath("res/textures/skybox/day/right.jpg"),
        getAbsolutePath("res/textures/skybox/day/left.jpg"),
        getAbsolutePath("res/textures/skybox/day/top.jpg"),
        getAbsolutePath("res/textures/skybox/day/bottom.jpg"),
        getAbsolutePath("res/textures/skybox/day/front.jpg"),
        getAbsolutePath("res/textures/skybox/day/back.jpg")
    };

    std::vector<std::string> nightFaces
    {
        getAbsolutePath("res/textures/skybox/night/right.jpg"),
        getAbsolutePath("res/textures/skybox/night/left.jpg"),
        getAbsolutePath("res/textures/skybox/night/top.jpg"),
        getAbsolutePath("res/textures/skybox/night/bottom.jpg"),
        getAbsolutePath("res/textures/skybox/night/front.jpg"),
        getAbsolutePath("res/textures/skybox/night/back.jpg")
    };

    Skybox daySkybox(dayFaces, skyboxShader);
    Skybox nightSkybox(nightFaces, skyboxShader);

    Shader modelShader("res/shaders/common.vertex.shader", "res/shaders/common.fragment.shader");
    Model catModel("res/models/cat/cat.obj");
    Cat cat(modelShader, catModel, glm::vec3(0.0f, 100.0f, 0.0f));

    Model boardModel("res/models/board/board.obj");
    Board board(modelShader, boardModel, glm::vec3(0.0f));

    Shader bezierShader("res/shaders/bezier.vertex.shader", "res/shaders/common.fragment.shader");
    Texture2D bezierTexture("res/textures/container.jpg");

    glm::mat4 controlPoints(0.0f);
    controlPoints[0][3] = 1.0f;
    controlPoints[0][2] = 2.0f;

    BezierGPU bezier(bezierShader, bezierTexture, controlPoints, 32);

    Shader sphereShader("res/shaders/sphere.vertex.shader", "res/shaders/sphere.fragment.shader");
    Model sphereModel("res/models/sphere/sphere.obj");
    Sphere sphere1(sphereShader, sphereModel, spherePosition1);
    Sphere sphere2(sphereShader, sphereModel, spherePosition2);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_Camera.GetViewMatrix();
        glm::mat4 projection = m_Camera.GetProjectionMatrix();


        cat.Draw(m_Light, m_Camera, m_Weather, deltaTime);
        board.Draw(m_Light, m_Camera, m_Weather, deltaTime);
        bezier.Draw(m_Light, m_Camera, m_Weather, deltaTime);
        sphere1.Draw(m_Light, m_Camera, m_Weather, deltaTime);
        sphere2.Draw(m_Light, m_Camera, m_Weather, deltaTime);

        // day/night
        if(m_Weather.IsDay()) daySkybox.Draw(m_Camera);
        else nightSkybox.Draw(m_Camera);
   

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void Scene::ChangeCameraType()
{
    if (m_CameraType == CameraType::STATIC) m_CameraType = CameraType::FOLLOWING;
    else if (m_CameraType == CameraType::FOLLOWING) m_CameraType = CameraType::FREE;
    else if (m_CameraType == CameraType::FREE) m_CameraType = CameraType::MOVINGOBJECT;
    else if (m_CameraType == CameraType::MOVINGOBJECT) m_CameraType = CameraType::STATIC;

    if (m_CameraType == CameraType::STATIC)
    {
        m_Camera.SetNewPosition(m_DefaultPosition, m_DefaultPitch, m_DefaultYaw);
    }

    m_Camera.SetCameraType(m_CameraType);
}
