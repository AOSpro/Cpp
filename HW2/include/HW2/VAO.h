#ifndef VAO_H
#define VAO_H
#include <glad/glad.h>
class VAO {
    public:
        GLuint ID;
        VAO();
        void bind() const;
        void unbind() const;
        void deleteVAO() const;
};
#endif
