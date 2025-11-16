// Start:🕒 2025-11-14 Friday 22:40:21
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "Camera.h"
Camera::Camera(int width,int height,glm::vec3 pos){
    Camera::width=width;
    Camera::height=height;
    Pos=pos;
}
void Camera::Matrix(float FOVdeg,float nearPlane,float farPlne,Shader& shader,const char* uniform){
    glm::mat4 view=glm::mat4(1.f);
    glm::mat4 proj=glm::mat4(1.f);
    view=glm::lookAt(Pos,Pos+Ori,Up);
    proj=glm::perspective(glm::radians(FOVdeg),(float)(width/height),nearPlane,farPlne);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID,uniform),1,GL_FALSE,glm::value_ptr(proj*view));
}
void Camera::Inputs(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
        Pos+=speed*Ori;
    }
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
        Pos+=speed*-Ori;
    }
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
        Pos+=speed*glm::normalize(glm::cross(Ori,Up));
    }
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
        Pos+=speed*-glm::normalize(glm::cross(Ori,Up));
    }
    if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS){
        Pos+=speed*Up;
    }
    if(glfwGetKey(window,GLFW_KEY_TAB)==GLFW_PRESS){
        Pos+=speed*-Up;
    }
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS){
        speed=.009f;
    }
    else if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS){
        speed=.001f;
    }
}
