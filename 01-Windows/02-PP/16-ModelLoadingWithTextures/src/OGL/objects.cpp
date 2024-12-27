
#include "objects.h"

namespace OGL{

GLfloat angle;

// functions: 
Mesh  createMesh(Geometry geometry, Material material)
{
    Mesh mesh;
    mesh.geometry = geometry;
    mesh.material = material;
    return mesh;
}

void displayObject(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Mesh object)
{
     
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(object.material.shaderProgramObject);
    // modelMatrix, translateMatrix and rotateMatrix
    object.modelMatrix = glm::mat4(1.0f);
        
    object.modelMatrix = glm::translate(object.modelMatrix,glm::vec3(0.0f,0.0f,-4.0f));
    object.modelMatrix = glm::rotate(object.modelMatrix, angle, glm::vec3(1.0f,0.0f,0.0f));
    object.modelMatrix = glm::rotate(object.modelMatrix, angle, glm::vec3(0.0f,1.0f,0.0f));
    object.modelMatrix = glm::rotate(object.modelMatrix, angle, glm::vec3(0.0f,0.0f,1.0f));

    // setuniformMaritx
    setUniformMatrix4fv(object.material.shaderProgramObject,"u_projectionMatrix" ,projectionMatrix);
    setUniformMatrix4fv(object.material.shaderProgramObject,"u_viewMatrix" ,viewMatrix);
    setUniformMatrix4fv(object.material.shaderProgramObject,"u_modelMatrix" ,object.modelMatrix);

    // bind vao
    glBindVertexArray(object.VAO);
    // binf ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.indicesEBO);
    // draw elements
    glDrawElements(GL_POINTS, object.geometry.indices.size(), GL_UNSIGNED_INT, 0);
    // unbind ebo,vao,program
    unbindBuffer();
    unbindVertexArray();
    glUseProgram(0);

}
void update(GLfloat elapsedTime)
{
    angle += elapsedTime;
}

void cleanup(void)
{

}}