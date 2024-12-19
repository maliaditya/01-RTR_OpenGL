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
    extern void displayBasic(Camera camera,Mesh mesh);
    extern void updateBasic(float elapsedTime);
}