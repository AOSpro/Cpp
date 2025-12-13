#include "HW2/VAO.h"
VAO::VAO() {
    glGenVertexArrays(1, &ID);
}
void VAO::bind() const {
    glBindVertexArray(ID);
}
void VAO::unbind() const {
    glBindVertexArray(0);
}
void VAO::deleteVAO() const {
    glDeleteVertexArrays(1, &ID);
}
