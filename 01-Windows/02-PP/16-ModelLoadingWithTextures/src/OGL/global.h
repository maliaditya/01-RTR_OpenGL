#pragma once

#include "precomp.h"

enum {
ATTRIBUTE_POSITION = 0,
ATTRIBUTE_COLOR,
ATTRIBUTE_NORMAL,
ATTRIBUTE_TEXCOORD,
};

// Define material types
enum MaterialType {
    BASIC_MATERIAL,
    PHONG_MATERIAL,
};

#define M_PI 3.14159265358979323846264338327950288


struct Geometry  // vertexdata
{
    std::string name;
    std::vector<glm::vec3> positions; 
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv; 
    std::vector<GLuint>    indices; 
};

// Define a generic Uniform structure for different types of uniforms
template <typename T>
struct Uniform {
    std::string name;
    T value;

    Uniform(const std::string& uniformName, const T& uniformValue)
        : name(uniformName), value(uniformValue) {}
};


// Define the Material structure to store shader program, uniforms, and attributes
struct Material {
    GLuint shaderProgramObject;
    MaterialType type;
    std::unordered_map<std::string, Uniform<float>> uniforms; // Example: float uniforms
    std::unordered_map<std::string, GLuint> attributes;    


    // Add uniforms of different types
    template <typename T>
    void addUniform(const std::string& name, const T& value) {
        uniforms[name] = Uniform<T>{name, value};  // Add uniform to the map
    }

    // Add attribute (GLuint location)
    void addAttribute(const std::string& name, GLuint location) {
        attributes[name] = location;
    }

};

struct Texture 
{
    GLuint colorMap = 0;
    GLuint alphaMap = 0;
    GLuint aoMap = 0;
    GLuint roughnessMap = 0;
    GLuint  metalnessMap = 0;
    GLuint normalMap = 0;
    GLuint displacementMap = 0;
    GLfloat displacementBias = 0.0f;
    GLfloat displacementScale = 0.0f;
    glm::vec2 repeatALL = glm::vec2(1,1);

};

// data: vertex data
struct Mesh {   
    std::string name;
    Geometry geometry;
    Material material;
    Texture texture;
    float angle_ = 0;
    
    // OpenGL buffers and objects
    GLuint VAO; 
    GLuint positionsVBO; 
    GLuint normalsVBO; 
    GLuint uvVBO; 
    GLuint indicesEBO;

    // Transformation properties

    glm::vec3 position = glm::vec3(0.0f); // Default translation
    glm::vec3 rotation = glm::vec3(0.0f);    // Default rotation
    glm::vec3 scale_ = glm::vec3(1,1,1);      // Default scale (scale is _scale in the variable name to avoid conflict)
    glm::mat4 modelMatrix = glm::mat4(1.0f);  // Default identity matrix
    glm::vec3 color = glm::vec3(1.0f);


    // Method to apply translation
    void translate(const glm::vec3& t)
    {
        position = t;
        updateModelMatrix();
    }

    // Method to apply scale
    void scale(const glm::vec3& t)
    {
        scale_ = t; // Fix the scale assignment
        updateModelMatrix();
    }

    // Method to apply rotation on the X axis
    void rotateX(float angle)
    {   
        rotation.x += angle;
        updateModelMatrix();
    }
    
    // Method to apply rotation on the Y axis
    void rotateY(float angle)
    {   
        rotation.y += angle;
        updateModelMatrix();
    }
    
    // Method to apply rotation on the Z axis
    void rotateZ(float angle)
    {   
        rotation.z += angle;
        updateModelMatrix();
    }
    // Method to apply the same rotation on all axes
    void rotate(float angle)
    {   
        angle_ += angle;
        rotation = glm::vec3(angle_, angle_, angle_);
        updateModelMatrix();
    }
    // Method to update the model matrix after transformations
    void updateModelMatrix()
    {
        // Reset model matrix to identity
        modelMatrix = glm::mat4(1.0f);
        // Apply translation
        modelMatrix = glm::translate(modelMatrix, position);
        // Apply rotations on each axis
        if (rotation.x != 0.0f)
        {
            modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X rotation
        }
        if (rotation.y != 0.0f)
        {
            modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y rotation
        }
        if (rotation.z != 0.0f)
        {
            modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z rotation
        }
        // Apply scaling
        modelMatrix = glm::scale(modelMatrix, scale_);
    }
    // Cleanup function to delete OpenGL resources
    void cleanup() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (positionsVBO != 0) glDeleteBuffers(1, &positionsVBO);
        if (normalsVBO != 0) glDeleteBuffers(1, &normalsVBO);
        if (uvVBO != 0) glDeleteBuffers(1, &uvVBO);
        if (indicesEBO != 0) glDeleteBuffers(1, &indicesEBO);
        if (material.shaderProgramObject != 0)
        {
            glUseProgram( material.shaderProgramObject);
            GLsizei numAttachedShaders = 0;
            glGetProgramiv(material.shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

            if (numAttachedShaders > 0)
            {
                std::vector<GLuint> shaderObjects(numAttachedShaders); // Automatic memory management
                glGetAttachedShaders(material.shaderProgramObject, numAttachedShaders, nullptr, shaderObjects.data());

                for (const auto& shader : shaderObjects)
                {
                    glDetachShader(material.shaderProgramObject, shader);
                    glDeleteShader(shader);
                }
            }

            glUseProgram(0);
            glDeleteProgram(material.shaderProgramObject);
        } 
        
        VAO = 0;
        positionsVBO = 0;
        normalsVBO = 0;
        uvVBO = 0;
        indicesEBO = 0;
        material.shaderProgramObject = 0;
    }
};


struct Light {
    glm::vec3 position;  // Position of the light
    glm::vec3 color;
                 // Color of the light

    // Example method to set light position
    void setPosition(const glm::vec3& newPosition) {
        position = newPosition;
    }

    // Example method to set light color
    void setColor(const glm::vec3& newColor) {
        color = newColor;
    }
};

struct Camera {
    
    glm::mat4 projectionMatrix = glm::mat4(1.0);  
    glm::mat4 transform = glm::mat4(1.0);  // Default identity matrix
    glm::vec3 position = glm::vec3(0.0f);  // Camera position
    glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);  // Default look-at direction
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Default up direction
    bool isRotating = false;
    GLfloat angleCamera;
    float verticalAngle = 0.0f; // Latitude
    GLfloat orbitRadius = 20.0f;

    // Calculate the view matrix
    glm::mat4 getViewMatrix(float interpolationFactor=0.1f) const {
        glm::vec3 interpolatedPos = glm::mix(position, target, 0.2f); // Interpolated position
        glm::vec3 interpolatedFront = glm::mix(target - position, target - interpolatedPos, 0.2f); 
    return glm::lookAt(interpolatedPos, interpolatedPos + glm::normalize(interpolatedFront), up);
}


    // Set the camera position
    void setPosition(const glm::vec3& newPosition) {
        position = newPosition;
        updateTransform();
    }

    // Set the target direction
    void setTarget(const glm::vec3& newTarget) {
        target = newTarget;
        updateTransform();
    }

    // Update the transformation matrix
    void updateTransform() {
        transform = glm::lookAt(position, target, up);
    }
};

struct Scene {
    std::vector<Mesh> meshes;  // List of meshes in the scene
    std::vector<Camera> cameras;  // List of cameras in the scene
    std::vector<Light> lights;  // List of lights in the scene
    Camera camera;  // Global camera matrix

    // Template function to add any type of object (Mesh, Camera, or Light)
    template <typename T>
    void add(const T& object)
    {
        if constexpr (std::is_same<T, Mesh>::value)
        {
            meshes.push_back(object); // Add mesh to the meshes vector
        }
        else if constexpr (std::is_same<T, Camera>::value)
        {
            camera = object;
            cameras.push_back(object); // Add camera to the cameras vector
        }
        else if constexpr (std::is_same<T, Light>::value)
        {
            lights.push_back(object); // Add light to the lights vector
        }
        else
        {
            static_assert(true, "Unsupported type for add function");
        }
    }


    // Get global camera matrix
    glm::mat4 getCamera() const
    {
        return camera.getViewMatrix();
    }

    // Get the number of meshes
    size_t getMeshCount() const
    {
        return meshes.size();
    }

    // Get the number of cameras
    size_t getCameraCount() const
    {
        return cameras.size();
    }

    // Get the number of lights
    size_t getLightCount() const
    {
        return lights.size();
    }
    
    void cleanupScene(std::vector<Mesh>& meshes) {
        
        for (auto& mesh : meshes) {
            mesh.cleanup(); // Call cleanup on each mesh
        }
        meshes.clear(); // Clear the scene vector after cleanup
    }


    // Clear all meshes, cameras, and lights
    void clearAll()
    {
        meshes.clear();
        cameras.clear();
        lights.clear();
    }
};


