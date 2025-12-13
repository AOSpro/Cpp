// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of planet
#ifndef PLANET_H
#define PLANET_H
#include <glm/gtc/matrix_transform.hpp>
#include "ball.h"
#include "textures.h"
#include "orbit.h"
#include "shaders.h"
class Planet {
    public:
        Ball sphere;
        Texture texture;
        Orbit orbit;
        float selfRotationSpeed;
        Planet(float radius,
                const char* texturePath,
                Orbit orb,
                float selfRot);
        void Draw(Shader& shader, float time, glm::mat4 parentTransform);
};
#endif
