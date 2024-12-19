#include "cubeGeometry.h"

namespace OGL {
    void generateCubeWithGridGeometry(
        float size,        // Cube size
        int gridSize,      // Number of divisions (grid size) per face
        std::vector<glm::vec3>& positions,  // Cube vertex positions
        std::vector<glm::vec3>& normals,    // Cube normals
        std::vector<glm::vec2>& uvs,        // Cube UV coordinates
        std::vector<GLuint>& indices        // Cube indices
    ) {
        positions.clear();
        normals.clear();
        uvs.clear();
        indices.clear();

        // Half size for positioning the cube in the range [-size/2, size/2]
        float halfSize = size / 2.0f;

        // Create the vertices for each face, applying grid divisions.
        for (int face = 0; face < 6; ++face) {
            glm::vec3 normal;

            // Determine the normal for each face
            if (face == 0) normal = glm::vec3(0.0f, 0.0f, -1.0f); // Front
            else if (face == 1) normal = glm::vec3(0.0f, 0.0f, 1.0f); // Back
            else if (face == 2) normal = glm::vec3(1.0f, 0.0f, 0.0f); // Right
            else if (face == 3) normal = glm::vec3(-1.0f, 0.0f, 0.0f); // Left
            else if (face == 4) normal = glm::vec3(0.0f, 1.0f, 0.0f); // Top
            else normal = glm::vec3(0.0f, -1.0f, 0.0f); // Bottom

            // Loop through grid rows and columns for this face
            for (int row = 0; row <= gridSize; ++row) {
                for (int col = 0; col <= gridSize; ++col) {
                    // Calculate the position of the vertex on the current face
                    float x = (col / float(gridSize)) * size - halfSize;
                    float y = (row / float(gridSize)) * size - halfSize;

                    // For different faces, assign the correct coordinates (x, y)
                    if (face == 0) positions.push_back(glm::vec3(x, y, -halfSize)); // Front face
                    else if (face == 1) positions.push_back(glm::vec3(x, y, halfSize)); // Back face
                    else if (face == 2) positions.push_back(glm::vec3(halfSize, y, -x)); // Right face
                    else if (face == 3) positions.push_back(glm::vec3(-halfSize, y, x)); // Left face
                    else if (face == 4) positions.push_back(glm::vec3(x, halfSize, -y)); // Top face
                    else positions.push_back(glm::vec3(x, -halfSize, y)); // Bottom face

                    // Push the normal vector for the current vertex
                    normals.push_back(normal);

                    // UV coordinates based on the grid for each face
                    float u = col / float(gridSize);
                    float v = row / float(gridSize);
                    uvs.push_back(glm::vec2(u, v));
                }
            }
        }

        // Generate indices for each face (2 triangles per face)
        for (int face = 0; face < 6; ++face) {
            for (int row = 0; row < gridSize; ++row) {
                for (int col = 0; col < gridSize; ++col) {
                    int topLeft = (face * (gridSize + 1) * (gridSize + 1)) + row * (gridSize + 1) + col;
                    int topRight = topLeft + 1;
                    int bottomLeft = topLeft + (gridSize + 1);
                    int bottomRight = bottomLeft + 1;

                    // First triangle (top-left, bottom-left, top-right)
                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(topRight);

                    // Second triangle (top-right, bottom-left, bottom-right)
                    indices.push_back(topRight);
                    indices.push_back(bottomLeft);
                    indices.push_back(bottomRight);
                }
            }
        }
    }



    

    Geometry cubeGeometry( float size  ,float gridSize)
    {
        Geometry geometry;
        geometry.name = "Cube";
        generateCubeWithGridGeometry(size,gridSize,  geometry.positions, geometry.normals, geometry.uv, geometry.indices  );
        return geometry;
    }

}