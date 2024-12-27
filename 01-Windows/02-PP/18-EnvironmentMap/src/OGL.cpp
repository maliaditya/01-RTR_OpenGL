#include "OGL.h"
#include <OGL/texture.h>
#include <OGL/model.h>
#include <OGL/shaders/phongADSLight.h>
#include <utils/combineStrings.h>

Scene scene;
Camera camera;
Logger ogl("OGL.log");



int initialize()
{   
    // Code
        std::vector<std::string> faces
    {
        std::string(RESOURCE_DIR)+"/textures/skybox/right.jpg",
        std::string(RESOURCE_DIR)+"/textures/skybox/left.jpg",
        std::string(RESOURCE_DIR)+"/textures/skybox/top.jpg",
        std::string(RESOURCE_DIR)+"/textures/skybox/bottom.jpg",
        std::string(RESOURCE_DIR)+"/textures/skybox/front.jpg",
        std::string(RESOURCE_DIR)+"/textures/skybox/back.jpg"
    };
    GLuint cubemapTexture = OGL::loadCubemap(faces); 

        
    const char* vertexShaderSource = R"(
        #version 460 core
        uniform mat4 u_projectionMatrix;
        uniform mat4 u_viewMatrix;
        uniform mat4 u_modelMatrix;

        in vec3 a_position;
        in vec3 a_normal;
        in vec2 a_uv;

        out vec2 a_uv_out;
        out vec3 a_position_out;
        out vec3 a_normal_out;

        void main()
        {

            
            a_normal_out = mat3(transpose(inverse(u_modelMatrix))) * a_normal;
            a_position_out = vec3(u_modelMatrix * vec4(a_position, 1.0));
            gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
            a_uv_out = a_uv;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 460 core
        uniform sampler2D texture1;

        uniform samplerCube skybox;
        uniform vec3 cameraPos;

        in vec2 a_uv_out;
        in vec3 a_position_out;
        in vec3 a_normal_out;

        out vec4 FragColor;
        void main()
        {
            vec3 I = normalize(a_position_out - cameraPos);
            vec3 R = reflect(I, normalize(a_normal_out));
            vec4 skyboxcolor = vec4(texture(skybox, R).rgb, 1.0);
            vec4 texturecolor = texture(texture1, a_uv_out);
            FragColor = texturecolor * skyboxcolor;
        }
    )";


    std::vector<Geometry> models = OGL::loadModel(std::string(RESOURCE_DIR)+"/models/helmet.glb");
    GLuint container = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/container.jpg");
    Geometry cubeGeometry =  models[0];
    Material shaderMaterial = OGL::shaderMaterial(vertexShaderSource, fragmentShaderSource);
    Mesh helmet = OGL::createMesh(cubeGeometry, shaderMaterial);
    helmet.name = "helmet";
    helmet.texture.cubeMap = cubemapTexture;
    helmet.rotation.x = M_PI*0.5;
    helmet.texture.colorMap = cubeGeometry.texture.colorMap;
    helmet.translate(glm::vec3(0.0f,0.0f,0.0f));
    helmet.scale(glm::vec3(1,1,1));
    scene.add(helmet);



    const char* skyBoxVertexShaderSource = R"(
        #version 460 core
        uniform mat4 u_projectionMatrix;
        uniform mat4 u_viewMatrix;

        in vec3 a_position;

        out vec3 TexCoords;

        void main()
        {
            TexCoords = a_position;
            gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position, 1.0f);
        }
    )";

    const char* skyBoxFragmentShaderSource = R"(
        #version 460 core
        uniform samplerCube skybox;

        in vec3 TexCoords;
        out vec4 FragColor;
        void main()
        {
            FragColor = texture(skybox, TexCoords);
        }
    )";


    Geometry cubeGeom = OGL::cubeGeometry(75,1);
    Material skyboxShaderMaterial = OGL::shaderMaterial(skyBoxVertexShaderSource,skyBoxFragmentShaderSource);
    Mesh cube = OGL::createMesh(cubeGeom, skyboxShaderMaterial);
    cube.name = "skybox";
    cube.texture.cubeMap = cubemapTexture;
    cube.translate(glm::vec3(0.0f,0.0f,0.0f));
    cube.scale(glm::vec3(1,1,1));
    scene.add(cube);

    camera.orbitRadius = 5;
    camera.target = helmet.position;

    // Depth & Clear Color
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    
	// Enabling the texture
	glEnable(GL_TEXTURE_2D);

    // Enable Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Optional
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    return 0;
}

void display()
{
    // Code
    for(auto& mesh : scene.meshes)
    {   
        if(mesh.name == "helmet")
        {    

            
            glUseProgram(mesh.material.shaderProgramObject);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.colorMap);
            OGL::setUniform1i(mesh.material.shaderProgramObject, "u_colorMap", 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mesh.texture.cubeMap);
            OGL::setUniform1i(mesh.material.shaderProgramObject, "skybox", 1);
        
            OGL::setUniform3fv(mesh.material.shaderProgramObject, "cameraPos", camera.position);

            OGL::displayShader(camera, mesh);
                
        }

        if(mesh.name == "skybox")
        {       
            glUseProgram(mesh.material.shaderProgramObject);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mesh.texture.cubeMap);
            OGL::setUniform1i(mesh.material.shaderProgramObject, "skybox", 0);

            OGL::displayShader(camera, mesh);
                
        }
        else
        {
            glUseProgram(mesh.material.shaderProgramObject);
            OGL::displayBasic(camera, mesh);
        }
    }
}

void update()
{
}

void uninitialize()
{
    scene.cleanupScene(scene.meshes);
}
