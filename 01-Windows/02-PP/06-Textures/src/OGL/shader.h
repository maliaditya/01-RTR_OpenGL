#pragma once
#include "global.h"

extern Logger shaderlog; // Declaration of logger object

extern GLuint createShader(GLenum type, const char* source, const std::string& shaderName = "default");
extern GLuint linkShadersAndAttributes(GLuint vertexshader, GLuint fragmentShader, std::unordered_map<std::string, GLuint>& attributes, const std::string& shaderName = "default");
extern void deleteShaderProgramObject(GLuint shaderProgramObject);
extern void setUniformMatrix4fv(GLuint shaderProgramObject, const char* uniform, glm::mat4 matrix);

/*
Improvements:
Shader: add objectname while passing to shader;
*/