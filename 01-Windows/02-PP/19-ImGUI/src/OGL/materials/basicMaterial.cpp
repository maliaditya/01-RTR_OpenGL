    #include "basicMaterial.h"

namespace OGL{
    BasicShader basicShader;

    Material basicMaterial()
    {
        Material material;
        material.type = BASIC_MATERIAL;
        material.addAttribute("a_position", ATTRIBUTE_POSITION);
        material.addAttribute("a_normal", ATTRIBUTE_NORMAL);
        material.addAttribute("a_uv", ATTRIBUTE_TEXCOORD);
            
        //build and compile shaders
        GLuint vertexShader = createShader( GL_VERTEX_SHADER, basicShader.vs, "basicMaterialVS"); 
        GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER,  basicShader.fs, "basicMaterialFS"); 
        material.shaderProgramObject =  linkShadersAndAttributes(vertexShader,fragmentShader,  material.attributes);
        
        return(material);
    }

    float angleBasic;

    void displayBasic(Camera camera, Mesh mesh)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(mesh.material.shaderProgramObject);

        // setuniformMaritx
        setUniformMatrix4fv(mesh.material.shaderProgramObject,"u_projectionMatrix" , camera.projectionMatrix);
        setUniformMatrix4fv(mesh.material.shaderProgramObject,"u_viewMatrix" ,camera.getViewMatrix());
        setUniformMatrix4fv(mesh.material.shaderProgramObject,"u_modelMatrix" ,mesh.modelMatrix);
        setUniform2fv(mesh.material.shaderProgramObject,"u_repeatAll",mesh.texture.repeatALL);
        setUniform1f(mesh.material.shaderProgramObject,"u_displacementBias",mesh.texture.displacementBias);
        if(mesh.texture.alphaMap!=0)
        {
            setUniform1i(mesh.material.shaderProgramObject,"isAlphaMap",1);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.alphaMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_alphaMap", 6);
        }
        else
        {
            setUniform1i(mesh.material.shaderProgramObject,"isAlphaMap",0);
        }
        
        if(mesh.texture.colorMap!=0)
        {
            setUniform1i(mesh.material.shaderProgramObject,"isColorMap",1);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.colorMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_colorMap", 0);
        }
        else
        {
            setUniform1i(mesh.material.shaderProgramObject,"isColorMap",0);
        }
        setUniform3fv(mesh.material.shaderProgramObject,"u_color",mesh.color);


        if(mesh.texture.normalMap!=0)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.normalMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_normalMap", 1);
        }
        
        if(mesh.texture.aoMap!=0)
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.aoMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_aoMap", 2);
        }
        if(mesh.texture.roughnessMap!=0)
        {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.roughnessMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_roughnessMap", 3);
        }
        if(mesh.texture.roughnessMap!=0)
        {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.roughnessMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_metalnessMap", 4);
        }
        if(mesh.texture.normalMap!=0)
        {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.normalMap);
            setUniform1i(mesh.material.shaderProgramObject, "u_displacementMap", 5);
        }


         mesh.geometry.display(mesh.geometry.VAO);
        
        glUseProgram(0);


    }

    void updateBasic(GLfloat elapsedTime)
    {
        angleBasic += elapsedTime;
    }
    }

