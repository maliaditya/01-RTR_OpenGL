#pragma once
#include "global.h"

namespace OGL{
extern Logger shaderlog; // Declaration of logger object

extern GLuint createShader(GLenum type, const char* source, const std::string& shaderName = "default");
extern GLuint linkShadersAndAttributes(GLuint vertexshader, GLuint fragmentShader, std::unordered_map<std::string, GLuint>& attributes, const std::string& shaderName = "default");
extern void deleteShaderProgramObject(GLuint shaderProgramObject);
extern void setUniformMatrix4fv(GLuint shaderProgramObject, const char* uniform, glm::mat4 matrix);
extern void setUniform1i(GLuint shaderProgramObject, const char* uniform, GLuint textureUnit);
extern void setUniform1f(GLuint shaderProgramObject, const char* uniform, GLfloat value);
extern void setUniform2fv(GLuint shaderProgramObject, const char* uniform, glm::vec2 value);
extern void setUniform3fv(GLuint shaderProgramObject, const char* uniform, glm::vec3 value);
extern void setUniform4fv(GLuint shaderProgramObject, const char* uniform, glm::vec4 value);

}

/*
Improvements:
Shader: add objectname while passing to shader;
*/