// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "HW2/sun.h"
Sun::Sun() : sphere(2.0f, 72, 36) {}//size
void Sun::Draw(Shader& shader) {
    glm::mat4 model(1.0f);
    shader.setMat4("model", model);
//    shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 0.6f));//color
    shader.setVec3("objectColor", glm::vec3(1.f, 0.8f, 0.3f));//color
    shader.setBool("isSun", true);   // emissive flag
    sphere.Draw();
    shader.setBool("isSun", false);  // reset for other objects
}
