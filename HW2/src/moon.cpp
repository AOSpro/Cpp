// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "HW2/moon.h"
glm::vec3 Earth=glm::vec3(0.0f, 0.0f, 0.0f);
double sizeMoon=.1f;
float aMoon=2.0f,bMoon=1.5f;
float speedAroundEarth=-70.f,speedMoon=10.f;
const char* picMoon="assets/textures/moon.png";
Moon::Moon():Planet(sizeMoon,
            picMoon,
            Orbit(Earth,aMoon,bMoon,speedAroundEarth),speedMoon) {}
