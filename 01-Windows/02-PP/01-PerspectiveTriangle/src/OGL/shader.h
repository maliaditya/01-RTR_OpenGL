#pragma once
#include "global.h"

extern Logger shaderlog; // Declaration of logger object

GLuint createShader(GLenum type, const char* source);
GLuint linkShadersAndAttributes(GLuint vertexshader, GLuint fragmentShader, std::unordered_map<std::string, GLint>& attributes);
void deleteShaderProgramObject(GLuint shaderProgramObject);
void setUniformMatrix4fv(GLuint shaderProgramObject, const char* uniform, glm::mat4 matrix);
