
#include "torusGeometry.h"

namespace OGL{
    void generateTorusGeometry(
        float outerRadius,  // The radius from the center of the torus to the center of the tube
        float innerRadius,  // The radius of the tube
        int numMajor,       // Number of segments along the major circle
        int numMinor,       // Number of segments along the minor circle
        std::vector<glm::vec3>& positions, 
        std::vector<glm::vec3>& normals, 
        std::vector<glm::vec2>& uvs, 
        std::vector<GLuint>& indices
    ) {
        // Clear any existing data
        positions.clear();
        normals.clear();
        uvs.clear();
        indices.clear();

        const float majorStep = 2.0f * M_PI / numMajor; // Step size for the major circle (theta)
        const float minorStep = 2.0f * M_PI / numMinor; // Step size for the minor circle (phi)

        // Generate vertices for the torus
        for (int i = 0; i < numMajor; ++i) {
            float theta = i * majorStep;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            for (int j = 0; j < numMinor; ++j) {
                float phi = j * minorStep;
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);

                // Position on the torus
                float x = (outerRadius + innerRadius * cosPhi) * cosTheta;
                float y = (outerRadius + innerRadius * cosPhi) * sinTheta;
                float z = innerRadius * sinPhi;
                positions.push_back(glm::vec3(x, y, z));

                // Normal vector at the vertex
                glm::vec3 normal(cosPhi * cosTheta, cosPhi * sinTheta, sinPhi);
                normals.push_back(glm::normalize(normal));

                // UV coordinates (mapping [0, 1] space)
                float u = (float)i / numMajor;
                float v = (float)j / numMinor;
                uvs.push_back(glm::vec2(u, v));
            }
        }

    for (int i = 0; i < numMajor; ++i) {
        int nextMajor = (i + 1) % numMajor; // Wrap around major segments

        for (int j = 0; j < numMinor; ++j) {
            int nextMinor = (j + 1) % numMinor; // Wrap around minor segments

            // Compute vertex indices
            int current      = i * numMinor + j;
            int nextMinorIdx = i * numMinor + nextMinor;
            int nextMajorIdx = nextMajor * numMinor + j;
            int diagonalIdx  = nextMajor * numMinor + nextMinor;

            // First triangle
            indices.push_back(current);
            indices.push_back(nextMinorIdx);
            indices.push_back(nextMajorIdx);

            // Second triangle
            indices.push_back(nextMinorIdx);
            indices.push_back(diagonalIdx);
            indices.push_back(nextMajorIdx);
        }
    }
    }

    Geometry torusGeometry( float outerRadius,float innerRadius,int numMajor,int numMinor)
    {
        Geometry geometry;
        geometry.name = "Torus";
        generateTorusGeometry(outerRadius, innerRadius,numMajor, numMinor,geometry.positions, geometry.normals, geometry.uv, geometry.indices  );
        geometry.setupBuffers();
        return(geometry);
    }


}