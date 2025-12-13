#ifndef TEXTURES_H
#define TEXTURES_H
#include <glad/glad.h>
#include "stb/stb_image.h"
#include <iostream>
class Texture {
    public:
        GLuint ID;
        GLenum type;
        GLenum unit;
        Texture(const char* imagePath,
                GLenum texType,
                GLenum slot,
                GLenum format,
                GLenum pixelType);
        void bind() const;
        void unbind() const;
        void deleteTex() const;
};
#endif
