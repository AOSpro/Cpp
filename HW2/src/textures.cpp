#include "HW2/textures.h"
#include "HW2/aosIO.h"
using namespace aosio;
Texture::Texture(const char* imagePath,
        GLenum texType,
        GLenum slot,
        GLenum format,
        GLenum pixelType)
{
    type = texType;
    unit = slot;
    // Generate texture ID
    glGenTextures(1, &ID);
    glActiveTexture(unit);
    glBindTexture(type, ID);
    // Set texture parameters
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip image vertically
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data) {
        // If format not specified, guess based on channels
        GLenum internalFormat = format;
        if (nrChannels == 1) internalFormat = GL_RED;
        else if (nrChannels == 3) internalFormat = GL_RGB;
        else if (nrChannels == 4) internalFormat = GL_RGBA;
        glTexImage2D(type, 0, internalFormat, width, height, 0, internalFormat, pixelType, data);
        glGenerateMipmap(type);
    } else {
        error("Failed to load texture: ",imagePath);
    }
    stbi_image_free(data);
    glBindTexture(type, 0);
}
void Texture::bind() const {
    glActiveTexture(unit);
    glBindTexture(type, ID);
}
void Texture::unbind() const {
    glBindTexture(type, 0);
}
void Texture::deleteTex() const {
    glDeleteTextures(1, &ID);
}
