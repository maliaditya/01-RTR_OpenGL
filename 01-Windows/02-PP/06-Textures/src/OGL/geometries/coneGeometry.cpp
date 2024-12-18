
#include "coneGeometry.h"

namespace OGL{
    
    void generateConeGeometry(
        float radius, 
        float height, 
        int slices, 
        std::vector<glm::vec3>& positions, 
        std::vector<glm::vec3>& normals, 
        std::vector<glm::vec2>& uvs, 
        std::vector<GLuint>& indices
    ) {
        const float angleStep = 2.0f * glm::pi<float>() / slices; // Angle between each slice
        const glm::vec3 apex(0.0f, height, 0.0f);                // Apex of the cone
        const glm::vec3 baseCenter(0.0f, 0.0f, 0.0f);            // Center of the cone base

        // --- Clear any existing data ---
        positions.clear();
        normals.clear();
        uvs.clear();
        indices.clear();

        // --- Add apex vertex ---
        positions.push_back(apex);
        normals.push_back(glm::normalize(glm::vec3(0.0f, height, 0.0f))); // Approximate upward normal
        uvs.push_back(glm::vec2(0.5f, 1.0f));                            // Apex UV

        // --- Add base center vertex ---
        positions.push_back(baseCenter);
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f)); // Normal pointing down
        uvs.push_back(glm::vec2(0.5f, 0.5f));            // Base center UV

        // --- Add base vertices ---
        for (int i = 0; i < slices; ++i) {
            float angle = i * angleStep;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Base vertex position
            positions.push_back(glm::vec3(x, 0.0f, z));
            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f)); // Base normal (downward)
            uvs.push_back(glm::vec2((x / radius + 1.0f) * 0.5f, (z / radius + 1.0f) * 0.5f)); // Radial UV
        }

        // --- Add indices for the base (triangle fan) ---
        for (int i = 2; i < slices + 2; ++i) {
            int next = (i + 1 <= slices + 1) ? i + 1 : 2; // Wrap around to the first base vertex
            indices.push_back(1); // Base center index
            indices.push_back(i);
            indices.push_back(next);
        }

        // --- Add side vertices and indices ---
        for (int i = 2; i < slices + 2; ++i) {
            int next = (i + 1 <= slices + 1) ? i + 1 : 2; // Wrap around

            // Add side indices
            indices.push_back(0); // Apex index
            indices.push_back(i);
            indices.push_back(next);
        }
    }

    Geometry coneGeometry(float radius, float height, int slices)
    {
        Geometry geometry;
        generateConeGeometry(radius, height, slices, geometry.positions,geometry.normals,geometry.uv,geometry.indices);
        return(geometry);
    }

};