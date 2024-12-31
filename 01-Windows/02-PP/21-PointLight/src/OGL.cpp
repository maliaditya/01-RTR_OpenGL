
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
        uniform int lightEnabled;

        struct Material {
            sampler2D diffuse;
            sampler2D specular;
            float shininess;
        }; 


        struct DirLight {
            vec3 direction;
            
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        struct PointLight {
            vec3 position;
            
            float constant;
            float linear;
            float quadratic;
            
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        struct SpotLight {
            vec3 position;
            vec3 direction;
            float cutOff;
            float outerCutOff;
        
            float constant;
            float linear;
            float quadratic;
        
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;       
        };
        
        #define NR_POINT_LIGHTS 10
        
        in vec3 a_normal_out;
        in vec2 a_uv_out;
        in vec3 fragPos;
        
        uniform int dirLightEnabled;
        uniform int pointLightEnabled;
        uniform int spotLightEnabled;
        uniform int pointLightCount;
        uniform sampler2D colorMap;
        uniform vec3 viewPos;
        uniform DirLight dirLight;
        uniform PointLight pointLights[NR_POINT_LIGHTS];
        uniform SpotLight spotLight;
        uniform Material material;

        out vec4 FragColor;
        vec3 CalcDirLight(DirLight light, vec3 a_normal_out, vec3 viewDirection);
        vec3 CalcPointLight(PointLight pointLights, vec3 a_normal_out,vec3 fragPos, vec3 viewDirection);
        void main()
        {
            vec3 color = u_color;
            vec3 viewDirection = normalize(viewPos-fragPos);
            
            vec3 result;
            if(dirLightEnabled==1)
               // result = CalcDirLight(dirLight, a_normal_out, viewDirection);

            if(pointLightEnabled >=1)
            {
            for(int i=0;i<pointLightCount;i++)
                result +=  CalcPointLight(pointLights[i], a_normal_out,fragPos, viewDirection);
            }
            
            
            FragColor = vec4(result* color, 1.0f);
        }


        vec3 CalcPointLight(PointLight pointLights , vec3 normal,vec3 fragPos, vec3 viewDirection){
                
                float ambientStrength = 0.1;
                vec3 ambient = pointLights.ambient * ambientStrength;
                
                vec3 norm = normalize(a_normal_out);
                vec3 lightDirection = normalize(lightPosition-fragPos);
                float diff  = max(dot(norm,lightDirection),0.0);
                vec3 diffuse = diff * pointLights.diffuse;
                float specularStrength = 0.5;
                vec3 reflectDirection = reflect(-lightDirection,norm);
                float spec = pow(max(dot(viewDirection,reflectDirection),0.0),32);
                vec3 specular = specularStrength * spec * pointLights.specular;

                // attenuation
                float distance    = length(pointLights.position - fragPos);
                float attenuation = 1.0 / (pointLights.constant + pointLights.linear * distance + pointLights.quadratic * (distance * distance));    

                ambient  *= attenuation;  
                diffuse  *= attenuation;
                specular *= attenuation;  
                
                return (ambient + diffuse + specular);
        }


        vec3 CalcDirLight(DirLight dirLight, vec3 a_normal_out, vec3 viewDirection)
        {
            float ambientStrength = 0.1;
            vec3 ambient = dirLight.ambient * ambientStrength;
            
            vec3 norm = normalize(a_normal_out);
            vec3 lightDirection = normalize(-dirLight.direction);
            float diff  = max(dot(norm,lightDirection),0.0);
            vec3 diffuse = diff * dirLight.diffuse;

            float specularStrength = 0.5;
            vec3 reflectDirection = reflect(-lightDirection,norm);
            float spec = pow(max(dot(viewDirection,reflectDirection),0.0),32);
            vec3 specular = specularStrength * spec * dirLight.specular;

            return (ambient + diffuse + specular);
        }


    )";

    Geometry cubeGeometry = OGL::cubeGeometry(2);
    Material cubeshaderMaterial = OGL::shaderMaterial(cubevertexShaderSource,cubefragmentShaderSource,"cubeshaderMaterial");
    Mesh cubemesh = OGL::createMesh(cubeGeometry, cubeshaderMaterial);
    cubemesh.name = "Cube";
    cubemesh.position.x = -5;
    cubemesh.updateModelMatrix();
    scene.add(cubemesh);

    Geometry torusGeometry = OGL::torusGeometry();
    Mesh torusmesh = OGL::createMesh(torusGeometry, cubeshaderMaterial);
    torusmesh.name = "torus";
    torusmesh.position.x = 0;
    torusmesh.updateModelMatrix();
    scene.add(torusmesh);

    
    Geometry sphereGeometry = OGL::sphereGeometry(1.5);
    Mesh sphere = OGL::createMesh(sphereGeometry, cubeshaderMaterial);
    sphere.name = "sphere";
    sphere.position.x = 5;
    sphere.updateModelMatrix();
    scene.add(sphere);

    Geometry planeGeometry = OGL::planeGeometry(30,30);
    Mesh plane = OGL::createMesh(planeGeometry, cubeshaderMaterial);
    near
    plane.name = "plane";
    plane.position.y = -1.9;
    plane.rotateX(-1.5);
    scene.add(plane);

    DirectionalLight dirlight;
    scene.add(dirlight);

    PointLight pointlight1;
    pointlight1.position.y =1;
    scene.add(pointlight1);

    
    PointLight pointlight2;
    pointlight2.position.y = 1;
    pointlight2.color = glm::vec3(0.0f,1.0f,0.0f);
    pointlight2.enable();
    scene.add(pointlight2);

    PointLight pointlight3;
    pointlight3.position.y = 1;
    pointlight3.color = glm::vec3(1.0f,0.0f,0.0f);
    pointlight2.enable();
    scene.add(pointlight3);

    PointLight pointlight4;
    pointlight4.position.y = 1;
    pointlight4.color = glm::vec3(0.0f,0.0f,1.0f);
    pointlight4.enable();
    scene.add(pointlight4);
    /**
     * 
     *  Light
    //  */
    // const char* lightvertexShaderSource = R"(
    //     #version 460 core
    //     uniform mat4 u_projectionMatrix;
    //     uniform mat4 u_viewMatrix;
    //     uniform mat4 u_modelMatrix;

        
    //     in vec3 a_position;
    //     in vec3 a_normal;
    //     in vec2 a_uv;
    //     out vec2 a_uv_out;

    //     void main()
    //     {
    //         gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
    //         a_uv_out = a_uv;

    //     }
    // )";
    // const char* lightfragmentShaderSource = R"(
    //     #version 460 core

    //     // Uniforms
    //     uniform vec3 u_color;  // Color of the fading dot
    //     in vec2 a_uv_out;      // UV coordinates passed from the vertex shader
    //     out vec4 FragColor;    // Final color output

    //     void main()
    //     {
    //         // Disc
    //         float strength = distance(a_uv_out, vec2(0.5));
    //         strength *= 20.0;
    //         strength = 1.5 - strength;  // Make the disc less opaque towards the edges

    //         // Apply the color with the smooth transition, where areas outside the disc are fully transparent
    //         FragColor = vec4(u_color * vec3(strength), strength-1.0);  // Use 'smoke' as alpha to make the outer parts transparent
    //     }
    // )";

    // Geometry lightGeometry = OGL::planeGeometry(2,2);
    // Material lightshaderMaterial = OGL::shaderMaterial(lightvertexShaderSource,lightfragmentShaderSource,"lightshaderMaterial");
    // Mesh lightmesh = OGL::createMesh(lightGeometry, lightshaderMaterial);
    // lightmesh.name = "Light";
    // lightmesh.color = pointlight.color;
    // lightmesh.translate(pointlight.position);
    // scene.add(lightmesh);
    // lightColor = lightmesh.color;
    // lightPosition = lightmesh.position;
    
    /**
     * Camera
     * 
     */
    
    camera.orbitRadius = 20;
    camera.target = torusmesh.position;

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
        //sizes.dockedLeft = ImGui::GetWindowSize().x;
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
        if (ImGui::ColorEdit3("Light", glm::value_ptr(scene.directionalLights[0].color))) { // Modify light color
              scene.directionalLights[0].enable();
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
        //sizes.dockedBottom = ImGui::GetWindowSize().y;
        // Scene Information
        ImGui::Text("Scene Stats");
        ImGui::Text("Meshes: %d", scene.meshes.size());
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
            else
            {   
                // directional light
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"viewPos",camera.position);
                if(scene.directionalLights.size())
                {
                    for(auto& light: scene.directionalLights)
                    {
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,"dirLight.direction", light.direction);
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,"dirLight.ambient",light.ambientColor);
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,"dirLight.diffuse", light.diffuseColor);
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,"dirLight.specular", light.specularColor);
                        OGL::setUniform1i(mesh.material.shaderProgramObject,"dirLightEnabled",scene.directionalLights.size());

                    }
                }
                if(scene.pointLights.size())
                {
                    for(int i=0;i<scene.pointLights.size();i++)
                    {
                        PointLight light = scene.pointLights[i];
                        std::string base = "pointLights["+ std::to_string(i)+"]";
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,(base + ".position").c_str(), light.position);
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,(base+".ambient").c_str(),light.ambientColor);
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,(base+".diffuse").c_str(), light.diffuseColor);
                        OGL::setUniform3fv(mesh.material.shaderProgramObject,(base+".specular").c_str(), light.specularColor);
                        OGL::setUniform1f(mesh.material.shaderProgramObject, (base+".constant").c_str(), light.constantAttenuation);
                        OGL::setUniform1f(mesh.material.shaderProgramObject, (base+".linear").c_str(), light.linearAttenuation);
                        OGL::setUniform1f(mesh.material.shaderProgramObject, (base+".quadratic").c_str(), light.quadraticAttenuation);
                        OGL::setUniform1i(mesh.material.shaderProgramObject,"pointLightCount",scene.pointLights.size());
                        OGL::setUniform1i(mesh.material.shaderProgramObject,"pointLightEnabled",scene.pointLights.size());
                    }
                }
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"u_color",mesh.color);
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"lightColor", lightColor);
                OGL::setUniform3fv(mesh.material.shaderProgramObject,"lightPosition", lightPosition);
                OGL::setUniform1i(mesh.material.shaderProgramObject,"lightEnabled",lightEnabled);
                OGL::displayShader(camera,mesh);
            }
        }
       
    }
    
}
Timer t;
GLfloat angle_a;
GLfloat angletorus;

void update(void)
{
    // Code
  for(auto& mesh : scene.meshes)
    {  

      
        if(mesh.name == "torus" || mesh.name == "Cube" || mesh.name == "sphere" )
            mesh.rotate(angle_a);


    }

         //for(int i=0;i<scene.pointLights.size();i++)
            GLfloat radius = 5; 
            GLfloat centerOfcircleZ = camera.target.z; 
            GLfloat centerOfcircleX = camera.target.x; 
            scene.pointLights[0].position.x = radius * glm::sin(angletorus) + centerOfcircleX;
            scene.pointLights[0].position.z = radius * glm::cos(angletorus) + centerOfcircleZ;
            scene.pointLights[0].enable();

            radius = 8; 
             centerOfcircleZ = camera.target.z; 
            scene.pointLights[1].position.x = radius * glm::sin(angletorus) + centerOfcircleX;
            scene.pointLights[1].position.z = radius * glm::cos(angletorus) + centerOfcircleZ;
            scene.pointLights[1].enable();

              radius = 12; 
             centerOfcircleZ = camera.target.z; 
             centerOfcircleX = camera.target.x; 
            scene.pointLights[2].position.x = radius * glm::sin(-(angletorus)) + centerOfcircleX;
            scene.pointLights[2].position.z = radius * glm::cos(-(angletorus)) + centerOfcircleZ;
            scene.pointLights[2].enable();
    angle_a = 0.01;
    angletorus+=0.008;

}

void uninitialize(void)
{
    // Code
    
}