// Start:🕒 2025-12-11 Thursday 12:59:24
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
//📌 Header file of sun
#ifndef SUN_H
#define SUN_H
#include "ball.h"
#include "shaders.h"
class Sun {
    public:
        Ball sphere;
        Sun();
        void Draw(Shader& shader);
};
#endif
