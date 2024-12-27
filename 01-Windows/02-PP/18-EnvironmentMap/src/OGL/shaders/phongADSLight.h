const char * phongADSLight = R"(

#version 460 core
#define MAX_LIGHTS 3

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform vec3 u_la[2];
uniform vec3 u_ld[2];
uniform vec3 u_ls[2];
uniform vec4 u_lightPosition[2];

struct Light{
    vec3 u_la;
    vec3 u_ld;
    vec3 u_ls;
    vec4 u_lightPosition;
};

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_materialShininess;

uniform Light u_light[MAX_LIGHTS];


vec3 phongLight(Light light,vec3 a_normal, vec4 viewPosition)
{
    vec3 phong_ads_light;
    vec3 ambient = light.u_la * u_ka;

    // Diffused Light  
    mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
    vec3 transformedNormals = normalize(normalMatrix * a_normal);
    vec3 lightDirection = normalize(vec3(light.u_lightPosition) - viewPosition.xyz);
    vec3 diffuse = light.u_ld * u_kd * max(dot(transformedNormals, lightDirection), 0.0);
    
    // Specular Light 
    vec3 reflectionVector = reflect(-lightDirection,transformedNormals);
    vec3 viewerVector = normalize(-viewPosition.xyz);
    vec3 specular = light.u_ls * u_ks * pow(max(dot(reflectionVector,viewerVector),0.0), u_materialShininess);
    
    // Phong ADS Light
    phong_ads_light = ambient + diffuse + specular;

    return(phong_ads_light);

}

)";