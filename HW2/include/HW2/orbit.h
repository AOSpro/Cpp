// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of orbit
#ifndef ORBIT_H
#define ORBIT_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>
#include "shaders.h"
#include "VAO.h"
#include "VBO.h"
class Orbit {
    public:
        glm::vec3 center;   // for absolute use (Earth), keep (0,0,0) for relative (Moon)
        float aAxis;
        float bAxis;
        float speed;
        Orbit(glm::vec3 center, float aAxis, float bAxis, float speed);
        // For Earth (absolute world positioning)
        glm::mat4 getTransformAbsolute(float time) const;
        // For Moon (relative to parent transform)
        glm::mat4 getTransformRelative(float time) const;
        // Draw origin-centered ellipse; place via provided parent transform
        void Draw(Shader& shader, const glm::mat4& parentTransform);
    private:
        VAO vao;
        VBO* vbo;
        std::vector<float> vertices;
        void generateEllipse(); // generates origin-centered ellipse vertices
};
#endif
