#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <chrono>
#include "shaders.h"
#include "textures.h"
#include "stb_image.h"
#include "Camera.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
int main()
{
    // --- Initialize GLFW ---
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "HW1", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    // geometry helpers
    constexpr float s = 0.5f;
    // pendulum geometry (rod + bob)
    GLfloat pendulumVertices[] = {
        // rod
        -s/10.f, -1.0f, 0.0f,   0.7f,0.7f,0.7f,  0.0f,0.0f,
        s/10.f, -1.0f, 0.0f,   0.7f,0.7f,0.7f,  1.0f,0.0f,
        s/10.f,  0.0f, 0.0f,   0.7f,0.7f,0.7f,  1.0f,1.0f,
        -s/10.f,  0.0f, 0.0f,   0.7f,0.7f,0.7f,  0.0f,1.0f,
        // bob
        -s*0.3f, -1.2f, 0.0f,   0.9f,0.2f,0.2f,  0.0f,0.0f,
        s*0.3f, -1.2f, 0.0f,   0.9f,0.2f,0.2f,  1.0f,0.0f,
        s*0.3f, -1.5f, 0.0f,   0.9f,0.2f,0.2f,  1.0f,1.0f,
        -s*0.3f, -1.5f, 0.0f,   0.0f,0.2f,0.2f,  0.0f,1.0f
    };
    GLuint pendulumIndices[] = { 0,1,2, 2,3,0, 4,5,6, 6,7,4 };
    // create VAOs/VBOs/EBOs
    VAO pendulumVAO; pendulumVAO.Bind();
    VBO pendulumVBO(pendulumVertices, sizeof(pendulumVertices));
    EBO pendulumEBO(pendulumIndices, sizeof(pendulumIndices));
    pendulumVAO.LinkAttrib(pendulumVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    pendulumVAO.LinkAttrib(pendulumVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    pendulumVAO.LinkAttrib(pendulumVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    pendulumVAO.Unbind(); pendulumVBO.Unbind(); pendulumEBO.Unbind();
    // create a circle mesh for the clock face
    const int CIRCLE_SEG = 64;
    std::vector<float> circleVerts;
    std::vector<unsigned int> circleIdx;
    // center vertex (pos, color, uv)
    circleVerts.insert(circleVerts.end(), { 0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.90f, 0.5f, 0.5f });
    const float radius = 0.6f;
    for (int i = 0; i <= CIRCLE_SEG; ++i) {
        float theta = 2.0f * 3.14159265358979323846f * float(i) / float(CIRCLE_SEG);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        circleVerts.push_back(x);
        circleVerts.push_back(y);
        circleVerts.push_back(0.0f);
        circleVerts.push_back(0.8f);
        circleVerts.push_back(0.8f);
        circleVerts.push_back(0.8f);
        circleVerts.push_back((cosf(theta) + 1.0f) * 0.5f);
        circleVerts.push_back((sinf(theta) + 1.0f) * 0.5f);
    }
    for (unsigned int i = 1; i <= CIRCLE_SEG; ++i) {
        circleIdx.push_back(0);
        circleIdx.push_back(i);
        circleIdx.push_back(i + 1);
    }
    VAO circleVAO; circleVAO.Bind();
    VBO circleVBO(circleVerts.data(), circleVerts.size() * sizeof(float));
    EBO circleEBO(circleIdx.data(), circleIdx.size() * sizeof(unsigned int));
    circleVAO.LinkAttrib(circleVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    circleVAO.LinkAttrib(circleVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    circleVAO.LinkAttrib(circleVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    circleVAO.Unbind(); circleVBO.Unbind(); circleEBO.Unbind();
    auto makeHand = [&](float width, float length, const glm::vec3& col) {
        std::vector<float> hv = {
            -width, 0.0f, 0.0f,  col.r, col.g, col.b, 0.0f, 0.0f,
            width, 0.0f, 0.0f,  col.r, col.g, col.b, 1.0f, 0.0f,
            width, length,0.0f, col.r, col.g, col.b, 1.0f, 1.0f,
            -width, length,0.0f, col.r, col.g, col.b, 0.0f, 1.0f
        };
        std::vector<unsigned int> hi = { 0,1,2, 2,3,0 };
        return std::pair<std::vector<float>, std::vector<unsigned int>>(hv, hi);
    };
    // --- Compile shaders and load texture -----------------------------------------------------------
    Shader shader("shaders/default.vert", "shaders/default.frag");
    Texture pic("assets/textures/pic.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    pic.texUnit(shader, "tex0", 0);
    glEnable(GL_DEPTH_TEST);
    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    // create hand meshes (pivot at origin)
    auto hourData = makeHand(0.03f, 0.28f, glm::vec3(0.1f, 0.1f, 0.1f));
    VAO hourVAO; hourVAO.Bind();
    VBO hourVBO(hourData.first.data(), hourData.first.size() * sizeof(float));
    EBO hourEBO(hourData.second.data(), hourData.second.size() * sizeof(unsigned int));
    hourVAO.LinkAttrib(hourVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    hourVAO.LinkAttrib(hourVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    hourVAO.LinkAttrib(hourVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    hourVAO.Unbind(); hourVBO.Unbind(); hourEBO.Unbind();
    auto minData = makeHand(0.02f, 0.42f, glm::vec3(0.2f, 0.2f, 0.2f));
    VAO minVAO; minVAO.Bind();
    VBO minVBO(minData.first.data(), minData.first.size() * sizeof(float));
    EBO minEBO(minData.second.data(), minData.second.size() * sizeof(unsigned int));
    minVAO.LinkAttrib(minVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    minVAO.LinkAttrib(minVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    minVAO.LinkAttrib(minVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    minVAO.Unbind(); minVBO.Unbind(); minEBO.Unbind();
    auto secData = makeHand(0.01f, 0.50f, glm::vec3(0.9f, 0.1f, 0.1f));
    VAO secVAO; secVAO.Bind();
    VBO secVBO(secData.first.data(), secData.first.size() * sizeof(float));
    EBO secEBO(secData.second.data(), secData.second.size() * sizeof(unsigned int));
    secVAO.LinkAttrib(secVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    secVAO.LinkAttrib(secVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    secVAO.LinkAttrib(secVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    secVAO.Unbind(); secVBO.Unbind(); secEBO.Unbind();
    // --- Render loop -------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Activate();
        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");
        GLint modelLoc = glGetUniformLocation(shader.ID, "model");
        GLint useTexLoc = glGetUniformLocation(shader.ID, "useTexture");
        // draw textured clock face
        pic.Bind();
        glUniform1i(useTexLoc, 1);
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        circleVAO.Bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)circleIdx.size(), GL_UNSIGNED_INT, 0);
        // hands should use vertex color (disable texture)
        glUniform1i(useTexLoc, 0);
        // pendulum swing
        float tsec = (float)glfwGetTime();
        const float amplitude = glm::radians(25.0f);
        // pendulum frequency: f = 1 Hz => omega = 1*pi*f
        const float omega = 2.0f * 3.14159265358979323846f * 1.0f; // 1 Hz
        float swing = amplitude * sinf(omega * tsec);
        // smooth real-time clock using fractional seconds
        using namespace std::chrono;
        auto now_tp = system_clock::now();
        std::time_t t = system_clock::to_time_t(now_tp);
        std::tm* now = std::localtime(&t);
        auto ms = duration_cast<milliseconds>(now_tp.time_since_epoch()) % 1000;
        float frac = ms.count() / 1000.0f;
        float seconds = (float)now->tm_sec + frac;
        float minutes = (float)now->tm_min + seconds / 60.0f;
        float hours = (float)(now->tm_hour % 12) + minutes / 60.0f;
        int secondsInt = now->tm_sec;
        float secBase = glm::radians(- (float)secondsInt / 60.0f * 360.0f);
        float minBase = glm::radians(-minutes / 60.0f * 360.0f);
        float hourBase = glm::radians(-hours / 12.0f * 360.0f);
        // Do NOT sync hands with pendulum: hands driven only by real time
        float secAngle = secBase;
        float minAngle = minBase;
        float hourAngle = hourBase;
        // draw hands (translated slightly in +Z so they sit on top of the face)
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
        model = glm::rotate(model, hourAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        hourVAO.Bind(); glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
        model = glm::rotate(model, minAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        minVAO.Bind(); glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
        model = glm::rotate(model, secAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        secVAO.Bind(); glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // draw pendulum
        glm::vec3 pivot = glm::vec3(0.0f, -0.9f, 0.0f);
        model = glm::translate(glm::mat4(1.0f), pivot);
        model = glm::rotate(model, swing, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pendulumVAO.Bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(pendulumIndices)/sizeof(unsigned int)), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // cleanup
    pendulumVAO.Delete(); pendulumVBO.Delete(); pendulumEBO.Delete();
    circleVAO.Delete(); circleVBO.Delete(); circleEBO.Delete();
    hourVAO.Delete(); hourVBO.Delete(); hourEBO.Delete();
    minVAO.Delete(); minVBO.Delete(); minEBO.Delete();
    secVAO.Delete(); secVBO.Delete(); secEBO.Delete();
    pic.Delete(); shader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
