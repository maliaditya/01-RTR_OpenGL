/*
    plane
    
    Variables
    data : planePosition, planeColors, planeIndices
    buffers :  planeVAO, planePositionVBO, planeColorVBO, planeEBO,
    Shaders: planeVertexShaderSource, planeFragmentShaderSource, planeVertexShader, planeFragmentShader
    Programobject: planeShaderProgramObject
    
    Functions : planeInit(); planeDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); planeCleanup();
*/
#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "shader.h"
#include "buffer.h"

// Data
extern std::vector<GLfloat> planePosition, planeColors;
extern std::vector<GLuint>  planeIndices;
// Shader
extern const char* planeVertexShaderSource;
extern const char* planeVertexShaderSource;
extern GLuint planeVAO, planePositionVBO, planeColorVBO, planeEBO;
extern GLuint planeVertexShader, planeFragmentShader;
extern GLuint planeShaderProgramObject;
// Functions
extern void planeInit(void);
extern void planeDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
extern void planeCleanup(void); 




/*

    planeInit():
        shaders creation
        shaders compilation
        prelinking attribute 
        shaders linking

        delete planeVertexShader
        delete planeFragmentShader
        
        generate planeVAO
        generate planePositionVBO
        generate planeColorVBO
        generate planeEBO
        unbind

    planeDisplay(viewMatrix, projectionMatrix):
        
        useprogram planeShaderProgramObject
        modelmatrix = identity
        translateMatrix = modelmatrix * translate.z is -1

        setuniform4fv(modelmatrix)
        setuniform4fv(viewMatrix)
        setuniform4fv(projectionMatrix)

        bind vao
        bind ebo
        drawelements
        useprogram 0
    
    planeCleanup():
        delete planeVAO
        delete planePositionVBO
        delete planeColorVBO
        delete planeEBO
        delete planeShaderProgramObject
*/ 
