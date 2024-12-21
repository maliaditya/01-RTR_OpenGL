/*
PYRAMID
Includes: Timer time;
data: pyramidPositions; pyramidColors; pyramidIndices; pyramidTexcoords; pyramid Normals;
buffer: pyramidVAO; pyramidPositionsVBO; pyramidColorsVBO; pyramidIndicesEBO;
shader: pyramidVertexShaderSource; pyramidFragmentShaderSource; pyramidVertexShader; pyramidFragmentShader; attributes; pyramidShaderProgramObject;
update: angle;
functions : int pyramidInit(void); void pyramidDisplay(void); void pyramidUpdate(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); void pyramidCleanup(void);
*/
#pragma once
#include "global.h"

// data: 
extern std::vector<glm::vec3> pyramidPositions; 
extern std::vector<glm::vec3> pyramidColors; 
extern std::vector<glm::vec2> pyramidTexcoords; 
extern std::vector<glm::vec3> pyramidNormals;
extern std::vector<GLuint>pyramidIndices; 

// shader: 
extern const char* pyramidVertexShaderSource; 
extern const char* pyramidFragmentShaderSource; 
extern GLuint pyramidVertexShader; 
extern GLuint pyramidFragmentShader; 
extern std::unordered_map<std::string, GLuint> attributes; 
extern GLuint pyramidShaderProgramObject;

// buffer:
extern GLuint pyramidVAO; 
extern GLuint pyramidPositionsVBO; 
extern GLuint pyramidColorsVBO; 
extern GLuint pyramidTexcoordsVBO; 
extern GLuint pyramidNormalsVBO; 
extern GLuint pyramidIndicesEBO;

// update: 
extern GLfloat pyramidAngle;

// functions: 
extern int pyramidInit(void); 
extern void pyramidDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); 
extern void pyramidUpdate(GLfloat elapsedTime); 
extern void pyramidCleanup(void);




