// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "HW2/galaxy.h"
#include <glad/glad.h>
// Fullscreen quad vertices (NDC coordinates with texcoords)
static float quadVertices[] = {
    // positions   // texcoords
    -1.0f,  1.0f,  0.0f, 1.0f, // top-left
    -1.0f, -1.0f,  0.0f, 0.0f, // bottom-left
    1.0f, -1.0f,  1.0f, 0.0f, // bottom-right
    -1.0f,  1.0f,  0.0f, 1.0f, // top-left
    1.0f, -1.0f,  1.0f, 0.0f, // bottom-right
    1.0f,  1.0f,  1.0f, 1.0f  // top-right
};
Galaxy::Galaxy(const char* texturePath)
    : texture(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE)
{
    vao.bind();
    vbo = new VBO(quadVertices, sizeof(quadVertices));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // texcoord
    glEnableVertexAttribArray(1);
    vao.unbind();
}
void Galaxy::Draw(Shader& shader) {
    shader.use();
    shader.setInt("texture1", 0);
    texture.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.unbind();
}
