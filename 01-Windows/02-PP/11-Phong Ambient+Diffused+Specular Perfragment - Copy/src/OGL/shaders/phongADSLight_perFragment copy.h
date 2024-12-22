
const char* vs = R"(
#version 460 core

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

out vec4 viewPosition_out;
out vec3 a_normal_out;

void main()
{   
    vec4 modelPosition = u_modelMatrix * vec4(a_position, 1.0);
    vec4 viewPosition = u_viewMatrix * modelPosition;
    
    gl_Position = u_projectionMatrix * viewPosition;

    viewPosition_out = viewPosition; 
    a_normal_out = a_normal;  
}
)";

const char* fs = R"(
#version 460 core

in vec4 viewPosition_out;
in vec3 a_normal_out;

vec3 phong_ads_light;

uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec4 u_lightPosition;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_materialShininess;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;

out vec4 FragColor;

void main()
{   
    // Ambient Light
    vec3 ambient = u_la * u_ka;

    // Diffused Light  
    mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
    vec3 transformedNormals = normalize(normalMatrix * a_normal_out);
    vec3 lightDirection = normalize(vec3(u_lightPosition) - viewPosition_out.xyz);
    vec3 diffuse = u_ld * u_kd * max(dot(transformedNormals, lightDirection), 0.0);
    
    // Specular Light 
    vec3 reflectionVector = reflect(-lightDirection,transformedNormals);
    vec3 viewerVector = normalize(-viewPosition_out.xyz);
    vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector,viewerVector),0.0), u_materialShininess);
    
    // Phong ADS Light
    phong_ads_light = ambient + diffuse + specular;

    FragColor = vec4(phong_ads_light,1.0f);


}

)";