// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "HW2/planet.h"
Planet::Planet(float radius,
        const char* texturePath,
        Orbit orb,
        float selfRot)
    : sphere(radius, 72, 36),
    texture(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE),
    orbit(orb),
    selfRotationSpeed(selfRot)
{}
void Planet::Draw(Shader& shader, float time, glm::mat4 parentTransform) {
    texture.bind();
    shader.setInt("texture1", 0);
    glm::mat4 model = parentTransform * orbit.getTransformRelative(time);
    model = glm::rotate(model,
            glm::radians(time * selfRotationSpeed),
            glm::vec3( 0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(100.0f), glm::vec3(1.0f, 0.0f, 1.0f));
    shader.setMat4("model", model);
    sphere.Draw();
}
