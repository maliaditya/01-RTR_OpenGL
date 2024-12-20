#include "OGL.h"
#include <OGL/texture.h>
#include <filesystem>

Scene scene;
Camera camera;
Timer timer;
Logger ogl("OGL.log");

GLfloat angleCamera = 0;

struct Cursor{
    double x; 
    double y;
    double lastx;
    double lasty;

}cursor;

struct Sizes{
    int width = WINWIDTH;
    int height = WINHEIGHT;
}sizes;

int initialize()
{   
    // Code
    std::filesystem::path currentPath = std::filesystem::current_path();
    ogl.debug("std::string(RESOURCE_DIR) : ",std::string(RESOURCE_DIR) );
    
    GLuint alpha =  loadTexture(std::string(RESOURCE_DIR) + "/textures/floor/alpha.jpg");
    GLuint floor_norm =  loadTexture(std::string(RESOURCE_DIR) + "/textures/sand_and_rocks/coast_sand_rocks_02_nor_gl_1k.jpg");
    GLuint floor_diff =  loadTexture(std::string(RESOURCE_DIR) + "/textures/sand_and_rocks/coast_sand_rocks_02_diff_1k.jpg");
    GLuint floor_arm =   loadTexture(std::string(RESOURCE_DIR) + "/textures/sand_and_rocks/coast_sand_rocks_02_arm_1k.jpg");
    GLuint floor_disp =  loadTexture(std::string(RESOURCE_DIR) + "/textures/sand_and_rocks/coast_sand_rocks_02_disp_1k.jpg");

    //Geometry PlaneGeometry = planeGeometry();
    // Geometry coneGeometry = OGL::coneGeometry(0.5,1,13);
    Geometry planeGeometry = OGL::planeGeometry(20, 20, 100, 100);
    Material basicMaterial = OGL::basicMaterial();
    
    Mesh floor = createMesh(planeGeometry, basicMaterial);
    floor.name = "cone";
    floor.texture.alphaMap = alpha;
    floor.texture.colorMap = floor_diff;
    floor.texture.normalMap = floor_norm;
    floor.texture.aoMap = floor_arm;
    floor.texture.metalnessMap = floor_arm;
    floor.texture.roughnessMap = floor_arm;
    floor.texture.displacementMap = floor_disp;
    floor.texture.displacementBias = 0.8;
    floor.texture.displacementMap = 5.0;
    floor.texture.repeatALL = glm::vec2(8,8);
    floor.rotation.x = -M_PI*0.5;
    floor.translate(glm::vec3(0.0f, -1.0f, -10.0f));
    floor.scale(glm::vec3(1,1,1));
    scene.add(floor);

    GLuint wall_diff =  loadTexture(std::string(RESOURCE_DIR) + "/textures/wall/castle_brick_broken_06_diff_1k.jpg");
    GLuint wall_arm =   loadTexture(std::string(RESOURCE_DIR) + "/textures/wall/castle_brick_broken_06_arm_1k.jpg");
    //GLuint wall_disp =  loadTexture(std::string(RESOURCE_DIR) + "/textures/wall/castle_brick_broken_06_disp_1k.jpg");
    GLuint wall_norm =  loadTexture(std::string(RESOURCE_DIR) + "/textures/wall/castle_brick_broken_06_nor_gl_1k.jpg");

    Geometry cubeGeometry = OGL::cubeGeometry(4,5);
    Mesh walls = createMesh(cubeGeometry, basicMaterial);
    walls.name = "cube";
    walls.texture.colorMap = wall_diff;
    walls.texture.normalMap = wall_norm;
    walls.texture.aoMap = wall_arm;
    walls.texture.metalnessMap = wall_arm;
    walls.texture.roughnessMap = wall_arm;
    walls.texture.displacementMap = wall_norm;
    walls.translate(glm::vec3(0.0f,1.25f,-10.0f));
    walls.scale(glm::vec3(1,1,1));
    scene.add(walls);

    GLuint roof_diff =  loadTexture(std::string(RESOURCE_DIR) + "/textures/roof/roof_slates_02_diff_1k.jpg");
    GLuint roof_arm =   loadTexture(std::string(RESOURCE_DIR) + "/textures/roof/roof_slates_02_arm_1k.jpg");
    //GLuint roof_disp =  loadTexture(std::string(RESOURCE_DIR) + "/textures/roof/roof_slates_02_disp_1k.jpg");
    GLuint roof_norm =  loadTexture(std::string(RESOURCE_DIR) + "/textures/roof/roof_slates_02_nor_gl_1k.jpg");

    Geometry pyramidGeometry = OGL::coneGeometry(4,2,4);
    Mesh roof = createMesh(pyramidGeometry, basicMaterial);
    roof.name = "roof";
    roof.texture.colorMap = roof_diff;
    roof.texture.normalMap = roof_norm;
    roof.texture.aoMap = roof_arm;
    roof.texture.metalnessMap = roof_arm;
    roof.texture.roughnessMap = roof_arm;
    roof.texture.displacementMap = roof_norm;
    roof.texture.repeatALL = glm::vec2(1,1);
    roof.rotation.y = M_PI * 0.75;
    roof.translate(glm::vec3(0.0f, 3.0f, -10.0f));
    roof.scale(glm::vec3(1,1,1));
    scene.add(roof);

    GLuint alphadoor =  loadTexture(std::string(RESOURCE_DIR) + "/textures/door/alpha.jpg");
    GLuint door_norm =  loadTexture(std::string(RESOURCE_DIR) + "/textures/door/normal.jpg");
    GLuint door_diff =  loadTexture(std::string(RESOURCE_DIR) + "/textures/door/color.jpg");
    GLuint door_arm =   loadTexture(std::string(RESOURCE_DIR) + "/textures/door/ambientOcclusion.jpg");
    GLuint door_disp =  loadTexture(std::string(RESOURCE_DIR) + "/textures/door/height.jpg");
    GLuint door_roughness =  loadTexture(std::string(RESOURCE_DIR) + "/textures/door/roughness.jpg");
    GLuint door_metalness =  loadTexture(std::string(RESOURCE_DIR) + "/textures/door/metalness.jpg");
    
    Geometry doorGeometry = OGL::planeGeometry(2,3,10,10);
    Mesh door = createMesh(doorGeometry, basicMaterial);
    door.name = "door";
    door.texture.alphaMap = alphadoor;
    door.texture.colorMap = door_diff;
    door.texture.normalMap = door_norm;
    door.texture.aoMap = door_arm;
    door.texture.metalnessMap = door_metalness;
    door.texture.roughnessMap = door_roughness;
    door.texture.displacementMap = door_norm;
    door.translate(glm::vec3(0.0f,0.5f,-7.8f));
    door.scale(glm::vec3(1,1,1));
    scene.add(door);



    // Geometry sphereGeometry = OGL::sphereGeometry(0.5,36,20);
    // Mesh sphere = createMesh(sphereGeometry, cubeMaterial);
    // sphere.translate(glm::vec3(3.0f,1.2f,-6.0f));
    // sphere.rotate(1.0f);
    // sphere.scale(glm::vec3(1,1,1));
    // scene.add(sphere);

    // Geometry torusGeometry = OGL::torusGeometry(0.5f,0.2f,36,9);
    // Mesh torus = createMesh(torusGeometry, cubeMaterial);
    // torus.translate(glm::vec3(3.0f,-1.2f,-6.0f));
    // torus.rotate(1.0f);
    // torus.scale(glm::vec3(1,1,1));
    // scene.add(torus);


    camera.position = glm::vec3(0.0f,0.0f,10.0f);
    camera.target = glm::vec3(0.0f,0.0f,-10.0f);
    camera.updateTransform();

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
    resize(WINWIDTH, WINHEIGHT);
    return 0;
}

void display()
{
    // code

    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0; i<scene.meshes.size() ; i++)
    {    
        if(scene.meshes[i].material.type == BASIC_MATERIAL)
        {
            OGL::displayBasic(camera, scene.meshes[i]);
        }
    }
}

void update()
{
    GLfloat deltaTime = timer.getElapsedTime();
    GLfloat rotationSpeed = 0.2f;  // Adjust the speed as needed
    
    // Update rotation for each mesh in the scene
    for (auto& mesh : scene.meshes)
    {   
       if (mesh.name == "cone")
        {
            //mesh.rotateZ(deltaTime * rotationSpeed); // X-axis rotation
        }
        else if(mesh.name == "cube")
        {
           // mesh.rotateY(deltaTime * rotationSpeed); // X-axis rotation
        }
        else if(mesh.name == "roof") 
        {
            //mesh.rotateY(deltaTime * rotationSpeed); // X-axis rotation
        }
         else if(mesh.name == "door") 
        {
           // mesh.rotate(deltaTime * rotationSpeed); // X-axis rotation
        }
       
    }
}

void resize(int width, int height)
{
    sizes.width = width;
    sizes.height = height;
    glViewport(0, 0, width, height);
    camera.projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void mousemove(float xpos, float ypos)
{
    // code
    cursor.x = (xpos / sizes.width - 0.5);
    cursor.y = -(ypos / sizes.height - 1.0);
    
    if(camera.isRotating && xpos > cursor.lastx)
    {
        angleCamera +=  xpos * 0.0001;
    }
    if(camera.isRotating && xpos < cursor.lastx)
    {
        angleCamera -=  xpos * 0.0001;
    }
    
    GLfloat radius = 20.0f;
    GLfloat offset = -10.0f;
    camera.position.x = 20.0 * glm::sin(angleCamera);
    camera.position.z = 20.0 * glm::cos(angleCamera) + offset;

    if(!camera.isRotating)
    {   
        camera.position.z = 10;
        camera.position.x = glm::sin(cursor.x) * 5;
        
    }

    camera.position.y = cursor.y * 5;

    cursor.lastx = xpos;
    cursor.lasty = ypos;

}

void l_buttonDown(double xpos, double ypos)
{
   
    camera.isRotating = true;
}
void l_buttonUp(double xpos, double ypos)
{
   
    camera.isRotating = false;
}
void keydown(char keydown)
{
    // code
    ogl.debug("keydown : ", keydown);
    if(keydown == 'w' || keydown == 'W')
    {   
        camera.position.z -= 0.1;
    }

    if(keydown == 's' || keydown == 'S')
    {
        camera.position.z += 0.1;
    }

    if(keydown == 'e' || keydown == 'E')
    {
        angleCamera += 0.01;
    }
    if(keydown == 'q' || keydown == 'Q')
    {
        angleCamera -= 0.01;
    }
}






void uninitialize()
{
    scene.cleanupScene(scene.meshes);
}
