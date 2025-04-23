#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    // Vertex Array Object ID reference
    GLuint ID{ 0 };
    // Constructor: Generates VAO ID
    VAO();

    void genArray();
    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Render(const unsigned int&);
    void Delete();
};

#endif
