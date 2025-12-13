// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of main
#ifndef main_H
#define main_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "HW2/shaders.h"
#include "HW2/camera.h"
#include "HW2/sun.h"
#include "HW2/earth.h"
#include "HW2/moon.h"
#include "HW2/galaxy.h"
#include <iostream>
#include "HW2/aosIO.h"
using namespace aosio;
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
// camera
Camera camera(glm::vec3(0.0f, 1.0f, 11.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// freeze logic
bool freezeMotion = false;
float frozenTime = 0.0f;
// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll((float)yoffset);
}
void processInput(GLFWwindow *window, Earth& earth, Moon& moon);
// alignment check
bool isAligned(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float tolerance = 0.01f) {
    glm::vec3 ab = glm::normalize(b - a);
    glm::vec3 ac = glm::normalize(c - a);
    float dot = glm::dot(ab, ac);
    return std::fabs(dot - 1.0f) < tolerance;
}
// eclipse search
float findNearestSolarEclipse(Earth& earth, Moon& moon) {
    float current = glfwGetTime();
    for (float t = current; t < current + 200.0f; t += 0.05f) {
        glm::vec3 sunPos(0.0f);
        glm::vec3 earthPos = glm::vec3(earth.orbit.getTransformAbsolute(t)[3]);
        glm::vec3 moonPos  = glm::vec3((earth.orbit.getTransformAbsolute(t) *
                    moon.orbit.getTransformRelative(t))[3]);
        if (isAligned(sunPos, moonPos, earthPos, 0.01f)) {
            return t;
        }
    }
    return current;
}
float findNearestLunarEclipse(Earth& earth, Moon& moon) {
    float current = glfwGetTime();
    for (float t = current; t < current + 200.0f; t += 0.05f) {
        glm::vec3 sunPos(0.0f);
        glm::vec3 earthPos = glm::vec3(earth.orbit.getTransformAbsolute(t)[3]);
        glm::vec3 moonPos  = glm::vec3((earth.orbit.getTransformAbsolute(t) *
                    moon.orbit.getTransformRelative(t))[3]);
        if (isAligned(sunPos, earthPos, moonPos, 0.01f)) {
            return t;
        }
    }
    return current;
}
GLFWwindow* start(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
    if (window == NULL) {
        error("Failed to create GLFW window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        error("Failed to initialize GLAD");
        return nullptr;
    }
    glEnable(GL_DEPTH_TEST);
    return window;
}
#endif //main_H
