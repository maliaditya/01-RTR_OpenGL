#include "sphereGeometry.h"

namespace OGL{

    void generateSphereGeometry(
        float radius,         // The radius of the sphere
        int slices,           // The number of slices (vertical divisions, i.e., latitude lines)
        int stacks,           // The number of stacks (horizontal divisions, i.e., longitude lines)
        std::vector<glm::vec3>& positions,  // Vector to hold the positions of the vertices
        std::vector<glm::vec3>& normals,    // Vector to hold the normals of the vertices
        std::vector<glm::vec2>& uvs,        // Vector to hold the UV coordinates
        std::vector<GLuint>& indices        // Vector to hold the indices for drawing the sphere
    ) {
        positions.clear();
        normals.clear();
        uvs.clear();
        indices.clear();

        float phiStep = M_PI / stacks;     // Angle step for vertical slices (latitude)
        float thetaStep = 2.0f * M_PI / slices;  // Angle step for horizontal slices (longitude)

        // Generate vertices for the sphere
        for (int i = 0; i <= stacks; ++i) {
            float phi = i * phiStep;  // Vertical angle (latitude)
            for (int j = 0; j <= slices; ++j) {
                float theta = j * thetaStep;  // Horizontal angle (longitude)

                // Calculate the position of the vertex using spherical coordinates
                float x = radius * sin(phi) * cos(theta);
                float y = radius * cos(phi);
                float z = radius * sin(phi) * sin(theta);

                positions.push_back(glm::vec3(x, y, z));  // Add the position

                // Calculate the normal (normalized position on the unit sphere)
                normals.push_back(glm::normalize(glm::vec3(x, y, z)));

                // UV coordinates (latitude and longitude)
                uvs.push_back(glm::vec2((float)j / slices, (float)i / stacks));
            }
        }

        // Generate indices for the sphere's triangles (2 triangles per quad)
        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                int first = i * (slices + 1) + j;         // First vertex in the stack
                int second = first + slices + 1;          // Opposite vertex in the stack

                // Two triangles per quad (use the indices to form the triangles)
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }

    Geometry sphereGeometry(float radius, int slices, int stack)
    {
        Geometry geometry;
        geometry.name = "Sphere";
        generateSphereGeometry(radius,slices, stack,geometry.positions, geometry.normals,geometry.uv,geometry.indices);
        return(geometry);
    }

}