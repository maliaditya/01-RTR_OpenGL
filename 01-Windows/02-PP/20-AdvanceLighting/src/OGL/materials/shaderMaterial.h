#pragma once
#include <OGL/global.h>

namespace OGL{

    extern Material shaderMaterial(const char* vertexShaderSource,const char* fragmentShaderSource, std::string shaderName = "shaderMaterial",Material material= Material());
    extern void displayShader(Camera camera, Mesh mesh);
   
}

