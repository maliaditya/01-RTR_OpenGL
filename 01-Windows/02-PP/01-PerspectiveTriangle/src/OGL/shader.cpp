#include "shader.h"

Logger shaderlog("shader.log"); // Initialize logger here

GLuint createShader(GLenum type, const char* source)
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
                shaderlog.debug("Vertex Shader Compilation Log:", log.data());
            else if (type == GL_FRAGMENT_SHADER)
                shaderlog.debug("Fragment Shader Compilation Log:", log.data());
        }
    }
    return shader;
}

GLuint linkShadersAndAttributes(GLuint vertexshader, GLuint fragmentShader, std::unordered_map<std::string, GLint>& attributes)
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
            shaderlog.debug("Vertex Shader Compilation Log:", log.data());
        }
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
