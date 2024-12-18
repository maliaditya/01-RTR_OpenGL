#pragma once
#include "global.h"

// Variable declarations
// Update
extern GLfloat angle;

// Data
extern std::vector<GLfloat> cubePositions; 
extern std::vector<GLfloat> cubeColors; 
extern std::vector<GLfloat> cubeNormals;
extern std::vector<GLfloat> cubeTexCoords;
extern std::vector<GLuint>  cubeIndices;


// Buffers
extern GLuint cubeVAO; 
extern GLuint cubePositionVBO; 
extern GLuint cubeColorsVBO; 
extern GLuint cubeEBO;

// Shaders
extern const char* cubeVertexShaderSource; 
extern const char* cubeFragmentShaderSource; 
extern GLuint cubeVertexShader; 
extern GLuint cubeFragmentShader; 
extern GLuint cubeShaderProgramObject;

// Function Declaration
extern int cubeInit(void); 
extern void cubeDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
extern void cubeUpdate(GLfloat elapsedTime); 
extern void cubeCleanup(void);