#include "shaderMaterial.h"

namespace OGL{

    Material shaderMaterial(const char* vertexShaderSource,const char* fragmentShaderSource,Material material, const std::string& shaderName)
    {
        material.type = BASIC_MATERIAL;
        material.addAttribute("a_position", ATTRIBUTE_POSITION);
        material.addAttribute("a_normal", ATTRIBUTE_NORMAL);
        material.addAttribute("a_uv", ATTRIBUTE_TEXCOORD);

        //build and compile shaders
        GLuint vertexShader = createShader( GL_VERTEX_SHADER, vertexShaderSource, shaderName); 
        GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER,  fragmentShaderSource,shaderName); 
        material.shaderProgramObject =  linkShadersAndAttributes(vertexShader,fragmentShader,  material.attributes,shaderName);
        
        return(material);
    }


    void displayShader(Camera camera, Mesh mesh)
    {
          //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(mesh.material.shaderProgramObject);

        // setuniformMaritx
        setUniformMatrix4fv(mesh.material.shaderProgramObject,"u_projectionMatrix" , camera.projectionMatrix);
        setUniformMatrix4fv(mesh.material.shaderProgramObject,"u_viewMatrix" ,camera.getViewMatrix());
        setUniformMatrix4fv(mesh.material.shaderProgramObject,"u_modelMatrix" ,mesh.modelMatrix);
        
        
        mesh.geometry.display(mesh.geometry.VAO);
        
        glUseProgram(0);
    }
}