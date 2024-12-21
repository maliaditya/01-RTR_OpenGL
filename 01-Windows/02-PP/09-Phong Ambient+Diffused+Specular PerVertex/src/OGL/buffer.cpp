#include "buffer.h"
namespace OGL {
    void genVertexArrays(GLuint* vao)
    {
        glGenVertexArrays(1, vao);
        glBindVertexArray(*vao);
    }

    void genBuffers(GLuint* vbo, std::vector<GLfloat>& vertexData)
    {
        glGenBuffers(1, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    }

    void genBuffers(GLuint* vbo, std::vector<GLuint>& vertexData)
    {
        glGenBuffers(1, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexData.size() * sizeof(GLuint), vertexData.data(), GL_STATIC_DRAW);
    }

    void genBuffers(GLuint* vbo, std::vector<glm::vec3>& vertexData)
    {
        glGenBuffers(1, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), vertexData.data(), GL_STATIC_DRAW);
    }

    void genBuffers(GLuint* vbo, std::vector<glm::vec2>& vertexData)
    {
        glGenBuffers(1, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec2), vertexData.data(), GL_STATIC_DRAW);
    }


    void vertexAttribPointer(GLuint location, GLuint size, GLint stride)
    {
        glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, NULL);
        glEnableVertexAttribArray(location);
    }

    void unbindBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void unbindVertexArray()
    {
        glBindVertexArray(0);
    }
}