#include "planeGeometry.h"
namespace OGL{
    void generatePlaneGeometry(
        float width,                // Width of the plane
        float height,               // Height of the plane
        int numRows,                // Number of rows in the grid
        int numCols,                // Number of columns in the grid
        std::vector<glm::vec3>& positions,  // Vertex positions
        std::vector<glm::vec3>& normals,    // Vertex normals
        std::vector<glm::vec2>& uvs,        // Texture coordinates
        std::vector<GLuint>& indices        // Indices for the plane
    ) {
        positions.clear();
        normals.clear();
        uvs.clear();
        indices.clear();

        // Calculate the spacing between each vertex
        float stepX = width / (numCols - 1);
        float stepY = height / (numRows - 1);

        // Generate the grid of vertices
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                // Calculate the position of each vertex
                float x = j * stepX - width / 2.0f;
                float y = i * stepY - height / 2.0f;

                // Add the vertex position
                positions.push_back(glm::vec3(x, y, 0.0f));

                // Add the normal (pointing in the positive z direction, for a flat plane)
                normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

                // Add the texture coordinates (ranging from 0 to 1)
                float u = (float)j / (numCols - 1);
                float v = (float)i / (numRows - 1);
                uvs.push_back(glm::vec2(u, v));
            }
        }

        // Generate the indices for the grid of triangles
        for (int i = 0; i < numRows - 1; ++i) {
            for (int j = 0; j < numCols - 1; ++j) {
                // Get the four corners of the current grid square
                int topLeft = i * numCols + j;
                int topRight = topLeft + 1;
                int bottomLeft = (i + 1) * numCols + j;
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



    Geometry planeGeometry( float width ,float height,int numRows,int numCols)
    {
        Geometry geometry;
        geometry.name = "Plane";
        generatePlaneGeometry( width,  height,  numRows, numCols, geometry.positions, geometry.normals, geometry.uv, geometry.indices);
        return(geometry);
    }

}