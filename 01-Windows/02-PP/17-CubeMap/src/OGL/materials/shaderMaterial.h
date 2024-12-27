#pragma once
#include <OGL/global.h>

namespace OGL{

    extern Material shaderMaterial(const char* vertexShaderSource,const char* fragmentShaderSource,Material material= Material(), const std::string& shaderName = "shaderMaterial");
    extern void displayShader(Camera camera, Mesh mesh);
   
}

