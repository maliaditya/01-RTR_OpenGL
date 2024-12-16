#pragma once
#include <GL/glew.h>
#include <vector>

void genVertexArrays(GLuint* vao);
void genBuffers(GLuint* vbo, std::vector<GLfloat>& vertexData);
void genBuffers(GLuint* vbo, std::vector<GLint>& vertexData);
void vertexAttribPointer(GLuint location, GLuint size, GLint stride = 0);
void unbindBuffer();
void unbindVertexArray();
