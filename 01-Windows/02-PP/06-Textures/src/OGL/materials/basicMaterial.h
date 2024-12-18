#pragma once
#include <OGL/global.h>

/*
    shaderSource: basicMaterialVsSource basicMaterialFsSource
    Function: GLuint basicMaterial(Material material)
              {
                // Build and Complile
                //link program
                // return shaderProgramObject
               }
*/
namespace OGL{
    extern const char* basicMaterialVsSource;
    extern const char* basicMaterialFsSource;
    extern Material basicMaterial(void);
    extern void displayBasic(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Mesh object);
    extern void updateBasic(float elapsedTime);
}