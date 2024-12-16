// triangle.h
#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "shader.h"
#include "buffer.h"

// Declare the triangle shader sources
extern const char* triangleVertexShaderSource;
extern const char* triangleFragmentShaderSource;

// Declare the triangle data (positions, colors, etc.)
extern std::vector<GLfloat> trianglePositions;
extern std::vector<GLfloat> triangleColors;
extern std::vector<GLint> triangeIndices;

// Declare OpenGL objects (VAO, VBOs, EBO)
extern GLuint triangleVAO, triangleVBO, triangleColorVBO, triangleEBO;
extern GLuint triangleShaderProgramObject;
extern GLuint triangelVertexShader;
extern GLuint triangleFragmentShader;

// Function declarations
void triangleInit();
void triangleDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
void triangleCleanup();
