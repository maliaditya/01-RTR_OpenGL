#pragma once
#include "global.h"


// shader: 
extern const char* pyramidVertexShaderSource; 
extern const char* pyramidFragmentShaderSource; 

// update: 
extern GLfloat angle;

// functions: 
extern Mesh createMesh(Geometry geometry, Material material);
extern void display(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); 
extern void update(GLfloat elapsedTime); 
extern void cleanup(void);



