#include "VAO.h"

#include <iostream>
VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::genArray() {
    return;
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Render(const unsigned int& indexArraySize) {
    VAO::Bind();
    glDrawElements(GL_TRIANGLES, indexArraySize, GL_UNSIGNED_INT, 0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}
