// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌
#include "HW2/earth.h"
glm::vec3 Sun=glm::vec3(0.0f, 0.0f, 0.0f);
double sizeEarth=0.7f;
float aEarth=9.0f,bEarth=6.0f;
float speedAroundSun=-20.0f,speedEarth=70.0f;
const char* picEarth="assets/textures/earth.png";
Earth::Earth():Planet(sizeEarth,
            picEarth,
            Orbit(Sun,aEarth,bEarth,speedAroundSun),speedEarth) {}
