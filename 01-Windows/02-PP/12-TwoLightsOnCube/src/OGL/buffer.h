#pragma once
#include "global.h"
namespace OGL{
void genVertexArrays(GLuint* vao);
void genBuffers(GLuint* vbo, std::vector<GLfloat>& vertexData);
void genBuffers(GLuint* vbo, std::vector<glm::vec3>& vertexData);
void genBuffers(GLuint* vbo, std::vector<glm::vec2>& vertexData);
void genBuffers(GLuint* vbo, std::vector<GLuint>& vertexData);
void vertexAttribPointer(GLuint location, GLuint size, GLint stride = 0);
void unbindBuffer();
void unbindVertexArray();

}