// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of galaxy
#ifndef GALAXY_H
#define GALAXY_H
#include "shaders.h"
#include "textures.h"
#include "VAO.h"
#include "VBO.h"
class Galaxy {
    public:
        Galaxy(const char* texturePath);
        void Draw(Shader& shader);
    private:
        VAO vao;
        VBO* vbo;
        Texture texture;
};
#endif
