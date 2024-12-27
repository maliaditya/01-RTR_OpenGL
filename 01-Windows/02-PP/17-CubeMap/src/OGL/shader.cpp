#include "shader.h"
  namespace OGL{
    Logger shaderlog("Shader.log"); // Initialize logger here

    GLuint createShader(GLenum type, const char* source, const std::string& shaderName)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

            if (infoLogLength > 0)
            {
                std::vector<char> log(infoLogLength);
                glGetShaderInfoLog(shader, infoLogLength, nullptr, log.data());
                if (type == GL_VERTEX_SHADER)
                {
                    shaderlog.error(shaderName + " Vertex Shader \n", log.data());
                }
                else if (type == GL_FRAGMENT_SHADER)
                {
                    shaderlog.error(shaderName + " Fragment Shader \n", log.data());
                }
            }
        }
        {
            shaderlog.info( shaderName + " Compilation Success.","");
        }
        return shader;
    }

    GLuint linkShadersAndAttributes(GLuint vertexshader, GLuint fragmentShader, std::unordered_map<std::string, GLuint>& attributes, const std::string& shaderName)
    {
        GLuint shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexshader);
        glAttachShader(shaderProgramObject, fragmentShader);

        for (auto& [key, value] : attributes)
        {
            glBindAttribLocation(shaderProgramObject, value, key.c_str());
        }

        glLinkProgram(shaderProgramObject);
        GLint success;
        glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLint infoLogLength;
            glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

            if (infoLogLength > 0)
            {
                std::vector<char> log(infoLogLength);
                glGetProgramInfoLog(shaderProgramObject, infoLogLength, nullptr, log.data());
                shaderlog.error(shaderName + " Shader Compilation \n", log.data());
            }
        }
        else
        {
            shaderlog.info(shaderName + "  Program Compilation Success.","");
        }

        return shaderProgramObject;
    }

    void deleteShaderProgramObject(GLuint shaderProgramObject)
    {
        glUseProgram(shaderProgramObject);

        GLsizei numAttachedShaders = 0;
        glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

        if (numAttachedShaders > 0)
        {
            std::vector<GLuint> shaderObjects(numAttachedShaders); // Automatic memory management
            glGetAttachedShaders(shaderProgramObject, numAttachedShaders, nullptr, shaderObjects.data());

            for (const auto& shader : shaderObjects)
            {
                glDetachShader(shaderProgramObject, shader);
                glDeleteShader(shader);
            }
        }

        glUseProgram(0);
        glDeleteProgram(shaderProgramObject);
    }

    void setUniformMatrix4fv(GLuint shaderProgramObject, const char* uniform, glm::mat4 matrix)
    {
        GLint location = glGetUniformLocation(shaderProgramObject, uniform);
        if (location == -1)
        {
            shaderlog.debug("Uniform not found: ", uniform);
            return;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }


    void setUniform1i(GLuint shaderProgramObject, const char* uniform, GLuint value)
    {
        GLint location = glGetUniformLocation(shaderProgramObject, uniform);
        if (location == -1)
        {
            shaderlog.debug("Uniform not found: ", uniform);
            return;
        }
        
        // Set the uniform to refer to the given texture unit (e.g., textureUnit 0, 1, 2, etc.)
        glUniform1i(location, value);  // Use glUniform1i to set the uniform value as texture unit
    }



    void setUniform2fv(GLuint shaderProgramObject, const char* uniform, glm::vec2 value)
    {
        GLint location = glGetUniformLocation(shaderProgramObject, uniform);
        if (location == -1)
        {
            shaderlog.debug("Uniform not found: ", uniform);
            return;
        }
        
        // Set the uniform to refer to the given texture unit (e.g., textureUnit 0, 1, 2, etc.)
        glUniform2fv(location,1,glm::value_ptr(value));  // Use glUniform2fv to set the uniform value as texture unit
    }



    void setUniform3fv(GLuint shaderProgramObject, const char* uniform, glm::vec3 value)
    {
        GLint location = glGetUniformLocation(shaderProgramObject, uniform);
        if (location == -1)
        {
            shaderlog.debug("Uniform not found: ", uniform);
            return;
        }

        // Set the uniform to refer to the given texture unit (e.g., textureUnit 0, 1, 2, etc.)
        glUniform3fv(location,1,glm::value_ptr(value));  // Use glUniform2fv to set the uniform value as texture unit
    }

    

    void setUniform4fv(GLuint shaderProgramObject, const char* uniform, glm::vec4 value)
    {
        GLint location = glGetUniformLocation(shaderProgramObject, uniform);
        if (location == -1)
        {
            shaderlog.debug("Uniform not found: hahah ", uniform);
            return;
        }

        // Set the uniform to refer to the given texture unit (e.g., textureUnit 0, 1, 2, etc.)
        glUniform4fv(location,1,glm::value_ptr(value));  // Use glUniform2fv to set the uniform value as texture unit
    }


    void setUniform1f(GLuint shaderProgramObject, const char* uniform, GLfloat value)
    {
        GLint location = glGetUniformLocation(shaderProgramObject, uniform);
        if (location == -1)
        {
            shaderlog.debug("Uniform not found: ", uniform);
            return;
        }
        
        // Set the uniform to refer to the given texture unit (e.g., textureUnit 0, 1, 2, etc.)
        glUniform1f(location, value);  // Use glUniform1i to set the uniform value as texture unit
    }
}