// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "HW2/orbit.h"
Orbit::Orbit(glm::vec3 center, float aAxis, float bAxis, float speed)
    : center(center), aAxis(aAxis), bAxis(bAxis), speed(speed), vbo(nullptr)
{
    generateEllipse();
}
void Orbit::generateEllipse() {
    const int segments = 128;
    vertices.reserve((segments + 1) * 3);
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.1415926535f * float(i) / float(segments);
        float x = aAxis * std::cos(theta);
        float z = bAxis * std::sin(theta);
        vertices.push_back(x);      // origin-centered
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }
    vao.bind();
    vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    vao.unbind();
}
glm::mat4 Orbit::getTransformRelative(float time) const {
    float angle = glm::radians(time * speed);
    float x = aAxis * std::cos(angle);
    float z = bAxis * std::sin(angle);
    glm::mat4 model(1.0f);
    return glm::translate(model, glm::vec3(x, 0.0f, z)); // no center
}
glm::mat4 Orbit::getTransformAbsolute(float time) const {
    float angle = glm::radians(time * speed);
    float x = center.x + aAxis * std::cos(angle);
    float z = center.z + bAxis * std::sin(angle);
    glm::mat4 model(1.0f);
    return glm::translate(model, glm::vec3(x, center.y, z));
}
void Orbit::Draw(Shader& shader, const glm::mat4& parentTransform) {
    glm::mat4 model = parentTransform * glm::translate(glm::mat4(1.0f), center);
    shader.setMat4("model", model);
    shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // white line
    vao.bind();
    glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)(vertices.size() / 3));
    vao.unbind();
}
