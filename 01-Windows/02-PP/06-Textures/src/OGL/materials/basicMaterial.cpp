    #include "basicMaterial.h"

namespace OGL{
    const char* basicMaterialVsSource = R"(
    #version 460 core
    uniform mat4 u_projectionMatrix;
    uniform mat4 u_viewMatrix;
    uniform mat4 u_modelMatrix;
    in vec3 a_position;
    in vec3 a_normal;
    in vec2 a_uv;
    out vec3 a_position_out;
    out vec3 a_normal_out;
    out vec2 a_uv_out;
    void main()
    {   
        vec4 modelPosition = u_modelMatrix * vec4(a_position,1.0f);
        vec4 viewPosition  = u_viewMatrix * modelPosition;
        vec4 projectionPosition = u_projectionMatrix * viewPosition;
        
        gl_Position = projectionPosition;
        
        // size Attenuation
        gl_PointSize = 1.0f;
        gl_PointSize *= (1.0f/-viewPosition.z);
        
        // varying
        a_position_out = a_position;
        a_normal_out = a_normal;
        a_uv_out = a_uv;

    }
    )";

    const char* basicMaterialFsSource = R"(
    #version 460 core
    in vec3 a_position_out;
    in vec3 a_normal_out;
    in vec2 a_uv_out;
    out vec4 FragColor;
    void main()
    {   
        //float lightIntensity = max(dot(a_normal_out, normalize(vec3(0.0f, 0.0f, -1.0f))), 0.0f); // Simple lighting
        //FragColor = vec4(lightIntensity,lightIntensity,lightIntensity,1.0f);
        FragColor = vec4(a_uv_out,1.0f,1.0f);
    }
    )";

    Material basicMaterial()
    {
        Material material;
        material.attributes = {
                {"a_position", ATTRIBUTE_POSITION},
                {"a_normal", ATTRIBUTE_NORMAL},
                {"a_uv", ATTRIBUTE_TEXCOORD},
            };

        //build and compile shaders
        GLuint vertexShader = createShader( GL_VERTEX_SHADER, basicMaterialVsSource, "basicMaterialVS"); 
        GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER,  basicMaterialFsSource, "basicMaterialFS"); 
        material.shaderProgramObject =  linkShadersAndAttributes(vertexShader,fragmentShader,  material.attributes);
        
        return(material);
    }

    float angleBasic;

    void displayBasic(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Mesh object)
    {
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(object.material.shaderProgramObject);
        // modelMatrix, translateMatrix and rotateMatrix
        // object.modelMatrix = glm::mat4(1.0f);
        // object.modelMatrix = glm::translate(object.modelMatrix,glm::vec3(0.0f,0.0f,-4.0f));
        // object.modelMatrix = glm::rotate(object.modelMatrix, angleBasic, glm::vec3(1.0f,0.0f,0.0f));
        // object.modelMatrix = glm::rotate(object.modelMatrix, angleBasic, glm::vec3(0.0f,1.0f,0.0f));
        // object.modelMatrix = glm::rotate(object.modelMatrix, angleBasic, glm::vec3(0.0f,0.0f,1.0f));

        // setuniformMaritx
        setUniformMatrix4fv(object.material.shaderProgramObject,"u_projectionMatrix" ,projectionMatrix);
        setUniformMatrix4fv(object.material.shaderProgramObject,"u_viewMatrix" ,viewMatrix);
        setUniformMatrix4fv(object.material.shaderProgramObject,"u_modelMatrix" ,object.modelMatrix);


        // bind vao
        glBindVertexArray(object.VAO);
        // binf ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.indicesEBO);
        // draw elements
        glDrawElements(GL_TRIANGLES, object.geometry.indices.size(), GL_UNSIGNED_INT, 0);
        // unbind ebo,vao,program
        unbindBuffer();
        unbindVertexArray();
        glUseProgram(0);

    }

    void updateBasic(GLfloat elapsedTime)
    {
        angleBasic += elapsedTime;
    }
    }