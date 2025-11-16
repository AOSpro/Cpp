// Start:🕒 2025-11-14 Friday 22:40:21
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of Camera
#ifndef Camera_H
#define Camera_H
#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "shaders.h"
class Camera{
    public:
        glm::vec3 Pos;
        glm::vec3 Ori=glm::vec3(.0f,.0f,-1.f);
        glm::vec3 Up=glm::vec3(.0f,1.f,.0f);
        bool firstClick=true;
        int width,height;
        float speed=.001f;
        float sens=100.f;
        Camera(int width,int height,glm::vec3 Pos);
        void Matrix(float FOVdeg,float nearPlane,float farPlne,Shader& shader,const char* uniform);
        void Inputs(GLFWwindow* window);
};
#endif //Camera_H
