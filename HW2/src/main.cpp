#include"HW2/main.h"
int main() {
    info("HW2 By:1.Aasm Edrees 2.Hala Issa");
    info("t.me/aospro");
    GLFWwindow* window=start();
    if(!window){
        return -1;
    }
    Shader planetShader("shaders/default.vert", "shaders/default.frag");
    Shader galaxyShader("shaders/galaxy.vert", "shaders/galaxy.frag");
    Sun sun;
    Earth earth;
    Moon moon;
    Galaxy galaxy("assets/textures/galaxy.png");
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, earth, moon);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw galaxy background without depth test
        glDisable(GL_DEPTH_TEST);
        galaxyShader.use();
        galaxy.Draw(galaxyShader);
        glEnable(GL_DEPTH_TEST);
        // use planet shader
        planetShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        planetShader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
        planetShader.setVec3("viewPos", camera.Position);
        planetShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        float time;
        if (freezeMotion) {
            time = frozenTime;
        } else {
            time = glfwGetTime();
            frozenTime = time;
        }
        // Sun
        sun.Draw(planetShader);
        // Earth orbit ellipse
        earth.orbit.Draw(planetShader, glm::mat4(1.0f));
        glm::mat4 earthTransform = earth.orbit.getTransformAbsolute(time);
        earth.Draw(planetShader, time, glm::mat4(1.0f));
        // Moon orbit ellipse (relative to Earth)
        moon.orbit.Draw(planetShader, earthTransform);
        moon.Draw(planetShader, time, earthTransform);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    warn("Program closed.");
    return 0;
}
void processInput(GLFWwindow *window, Earth& earth, Moon& moon) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (!freezeMotion) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            freezeMotion = true;
            frozenTime = findNearestSolarEclipse(earth, moon);
            info("Frozen at nearest solar eclipse.");
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            freezeMotion = true;
            frozenTime = findNearestLunarEclipse(earth, moon);
            info("Frozen at nearest lunar eclipse.");
        }
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        freezeMotion = false;
        info("Motion resumed.");
    }
}
