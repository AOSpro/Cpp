// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of ball
#ifndef BALL_H
#define BALL_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Ball {
    public:
        Ball(float radius = 1.0f, unsigned int sectorCount = 36, unsigned int stackCount = 18);
        void Draw();
    private:
        float radius;
        unsigned int sectorCount;
        unsigned int stackCount;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        VAO vao;
        VBO* vbo;
        EBO* ebo;
        void generateSphere();
        void setupMesh();
};
#endif
