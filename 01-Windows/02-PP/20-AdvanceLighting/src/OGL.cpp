
#include "OGL.h"

Scene scene;
Camera camera;


glm::vec3 lightColor;
glm::vec3 lightPosition;
GLuint lightEnabled = 1;

Logger ogl;
std::string Logger::debugLogs;  // This is the definition outside the class

// Function Declaration
int initialize(void)
{
    // Code

    /**
     * Cube
     * 
     */

    const char* cubevertexShaderSource = R"(
        #version 460 core
        uniform mat4 u_projectionMatrix;
        uniform mat4 u_viewMatrix;
        uniform mat4 u_modelMatrix;
        

        in vec3 a_position;
        in vec3 a_normal;
        in vec2 a_uv;

        out vec3 a_normal_out;
        out vec2 a_uv_out;
        out vec3 fragPos;

        void main()
        {
            gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
            fragPos = vec3(u_modelMatrix * vec4(a_position, 1.0f));
            a_normal_out = mat3(transpose(inverse(u_modelMatrix)))*a_normal;
            a_uv_out = a_uv;
        }
    )";
    const char* cubefragmentShaderSource = R"(
        #version 460 core
        uniform vec3 u_color;
        uniform vec3 lightColor;
        uniform vec3 lightPosition;
        uniform vec3 viewPos;
        uniform int lightEnabled;
        uniform sampler2D colorMap;
        
        in vec3 a_normal_out;
        in vec2 a_uv_out;
        in vec3 fragPos;
        

        out vec4 FragColor;

        void main()
        {
            vec3 color = texture(colorMap,a_uv_out ).rgb;

            float ambientStrength = 0.1;
            vec3 ambientLight = lightColor * ambientStrength;
            
            vec3 norm = normalize(a_normal_out);
            vec3 lightDirection = normalize(lightPosition-fragPos);
            float diff  = max(dot(norm,lightDirection),0.0);
            vec3 diffuse = diff * lightColor;

            float specularStrength = 0.5;
            vec3 viewDirection = normalize(viewPos-fragPos);
            vec3 reflectDirection = reflect(-lightDirection,norm);
            float spec = pow(max(dot(viewDirection,reflectDirection),0.0),32);
            vec3 specular = specularStrength * spec * lightColor;

            vec3 result =   (ambientLight + diffuse + specular) * color ;
            if(lightEnabled == 1)
                FragColor = vec4(result, 1.0f);
            if(lightEnabled == 0)
                FragColor = vec4(ambientLight* color, 1.0f);
        }
    )";

    Geometry cubeGeometry = OGL::sphereGeometry(0.3);
    Material cubeshaderMaterial = OGL::shaderMaterial(cubevertexShaderSource,cubefragmentShaderSource,"cubeshaderMaterial");
    Mesh cubemesh = OGL::createMesh(cubeGeometry, cubeshaderMaterial);
    cubemesh.name = "Cube";
    cubemesh.texture.colorMap = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/planets/2k_earth_daymap.jpg");
    cubemesh.updateModelMatrix();
    scene.add(cubemesh);

    Geometry moonGeometry = OGL::sphereGeometry(0.08);
    Mesh moonmesh = OGL::createMesh(moonGeometry, cubeshaderMaterial);
    moonmesh.name = "moon";
    moonmesh.texture.colorMap = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/planets/2k_moon.jpg");
    moonmesh.updateModelMatrix();
    scene.add(moonmesh);

    
    Geometry nebulaGeometry = OGL::sphereGeometry(75);
    Mesh nebula = OGL::createMesh(nebulaGeometry, cubeshaderMaterial);
    nebula.name = "nebula";
    nebula.texture.colorMap = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/planets/miklyway.jpg");
    nebula.updateModelMatrix();
    scene.add(nebula);



    /**
     * 
     *  Light
     */
    const char* lightvertexShaderSource = R"(
        #version 460 core
        uniform mat4 u_projectionMatrix;
        uniform mat4 u_viewMatrix;
        uniform mat4 u_modelMatrix;

        
        in vec3 a_position;
        in vec3 a_normal;
        in vec2 a_uv;
        out vec2 a_uv_out;

        void main()
        {
            gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
            a_uv_out = a_uv;

        }
    )";
    const char* lightfragmentShaderSource = R"(
        #version 460 core
        
        uniform vec3 u_color;
        uniform sampler2D colorMap;
        in vec2 a_uv_out;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(texture(colorMap,a_uv_out ).rgb *u_color,1.0);
        }
    )";

    Geometry lightGeometry = OGL::sphereGeometry(0.8);
    Material lightshaderMaterial = OGL::shaderMaterial(lightvertexShaderSource,lightfragmentShaderSource,"lightshaderMaterial");
    Mesh lightmesh = OGL::createMesh(lightGeometry, lightshaderMaterial);
    lightmesh.name = "Light";
    lightmesh.color = glm::vec3(1.0,1.0,1.0f);
    lightmesh.texture.colorMap = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/planets/2k_sun.jpg");
    scene.add(lightmesh);
    lightColor = lightmesh.color;
    lightPosition = lightmesh.position;
    
    /**
     * Camera
     * 
     */
    
    camera.orbitRadius = 10;
    camera.target = lightmesh.position;

    // Example log generation

    // Depth
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    
    // Texture
    glEnable(GL_TEXTURE_2D);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Smooth
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Points
    glEnable(GL_PROGRAM_POINT_SIZE);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    return(0);
}


void display(void)
{

      if(GUI)
    {
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Mesh Manager", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        sizes.dockedLeft = ImGui::GetWindowSize().x;
            ImGui::Spacing();
            ImGui::Text("Scene Meshes");
            ImGui::Separator(); // Line under the header

            for (auto& mesh : scene.meshes) {
                ImGui::Spacing();

                // Unique ID for each mesh to avoid ImGui conflicts
                ImGui::PushID(mesh.name.c_str());

                // Create a horizontal layout
                ImGui::Text("%s", mesh.name.c_str()); // Display mesh name
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100.0f); // Align checkbox to the right

                bool isVisible = mesh.visible; // Get visibility state
                if (ImGui::Checkbox("Visible", &isVisible)) {
                    mesh.visible = isVisible; // Update visibility state
                    if(mesh.name == "Light" && mesh.visible)
                        {
                            lightEnabled=1;
                            ogl.Log("Light Enabled");
                        }
                     if(mesh.name == "Light" && !mesh.visible)
                        {
                            lightEnabled=0;
                             ogl.Log("Light Disabled");
                        }
                }

                ImGui::PopID(); // Restore ID stack
                ImGui::Separator(); // Line between each mesh
            }
        // Demo window content
        ImGui::End();
        // Example: Create a demo window
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("right", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
         // Light Color Picker
        ImGui::Text("Light Color");
        if (ImGui::ColorEdit3("Light", glm::value_ptr(lightColor))) { // Modify light color
            for (auto& mesh : scene.meshes) {
                if (mesh.name == "Light") {
                    mesh.color = lightColor; // Update light mesh color
                }
            }
        }

        // Cube Color Picker
        ImGui::Text("Cube Color");
        for (auto& mesh : scene.meshes) {
            if (mesh.name == "Cube") {
                if (ImGui::ColorEdit3("Cube", glm::value_ptr(mesh.color))) {
                    // Update cube material color
                   // mesh.updateColor(); // Assuming you have a method to update the color in the shader
                }
                break;
            }
        }
        ImGui::End();

        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Debugging Logs", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        sizes.dockedBottom = ImGui::GetWindowSize().y;
        // Scene Information
        ImGui::Text("Scene Stats");
        ImGui::Text("Meshes: %d", scene.meshes.size());
        ImGui::Text("Lights: %d", scene.lights.size());
        ImGui::Separator(); // Divider between sections

        // Debugging / Logging Section
        ImGui::Text("Debugging");
        ImGui::TextWrapped("%s", Logger::debugLogs.c_str());  // Display all logs

        ImGui::End();

     
    }

    // Code
    for(auto& mesh : scene.meshes)
    {   
        if(mesh.material.type = SHADER_MATERIAL)
        {   
            glUseProgram(mesh.material.shaderProgramObject);

            if(mesh.name == "Light" && lightEnabled)
            {
               OGL::setUniform3fv(mesh.material.shaderProgramObject,"u_color",lightColor);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,mesh.texture.colorMap );
                OGL::setUniform1i(mesh.material.shaderProgramObject,"colorMap",0);
                OGL::displayShader(camera,mesh);
            }
            if(mesh.name == "Cube" || mesh.name == "moon" || mesh.name =="nebula" && mesh.visible)
            {   
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"lightColor", lightColor);
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"lightPosition", lightPosition);
               // OGL::setUniform3fv(mesh.material.shaderProgramObject,"u_color",mesh.color);
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"viewPos",camera.position);
                OGL::setUniform1i(mesh.material.shaderProgramObject,"lightEnabled",lightEnabled);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,mesh.texture.colorMap );
                OGL::setUniform1i(mesh.material.shaderProgramObject,"colorMap",0);
                OGL::displayShader(camera,mesh);
            }

        }
            
        if(mesh.material.type = BASIC_MATERIAL)
            OGL::displayBasic(camera, mesh);
    }
    
}
Timer t;
GLfloat angleEarth;
GLfloat angleMoon;

void update(void)
{
    // Code
  for(auto& mesh : scene.meshes)
    {  
        if(mesh.name == "Cube")
        {
            GLfloat radius = 3; // how much away from the center
            GLfloat centerOfcircleZ = camera.target.z; // camera target is object position
            GLfloat centerOfcircleX = camera.target.x; // camera target is object position
            mesh.position.x = radius * glm::sin(angleEarth) + centerOfcircleX;
            mesh.position.z = radius * glm::cos(angleEarth) + centerOfcircleZ;
            mesh.updateModelMatrix();

            mesh.rotateY(t.getElapsedTime());
        }
         if(mesh.name == "moon")
        {
            GLfloat radius = 0.5; // how much away from the center
            GLfloat centerOfcircleZ = scene.meshes[0].position.z; // camera target is object position
            GLfloat centerOfcircleX = scene.meshes[0].position.x; // camera target is object position
            mesh.position.x = radius * glm::sin(angleMoon) + centerOfcircleX;
            mesh.position.z = radius * glm::cos(angleMoon) + centerOfcircleZ;
            mesh.updateModelMatrix();

            mesh.rotateY(t.getElapsedTime()*0.01);
        }
    }

    
    angleEarth += 0.001;
    angleMoon += 0.01;

}

void uninitialize(void)
{
    // Code
    
}