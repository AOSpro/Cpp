#include "HW2/EBO.h"
EBO::EBO(const void* indices, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
void EBO::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void EBO::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::deleteEBO() const {
    glDeleteBuffers(1, &ID);
}
