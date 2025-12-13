#ifndef VBO_H
#define VBO_H
#include <glad/glad.h>
class VBO {
    public:
        GLuint ID;
        VBO(const void* data, GLsizeiptr size);
        void bind() const;
        void unbind() const;
        void deleteVBO() const;
};
#endif
