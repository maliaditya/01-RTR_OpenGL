#pragma once
#include "global.h"

namespace OGL{
// shader: 
extern const char* pyramidVertexShaderSource; 
extern const char* pyramidFragmentShaderSource; 

// update: 

// functions: 
extern Mesh createMesh(Geometry geometry, Material material);
extern void display(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); 
extern void update(GLfloat elapsedTime); 
extern void cleanup(void);



}