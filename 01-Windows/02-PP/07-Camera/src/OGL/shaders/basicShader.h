struct BasicShader
{

   // Combine the strings
const char* combine(const char* str1, const char* str2) {
       size_t len1 = strlen(str1);
       size_t len2 = strlen(str2);
       char* result = new char[len1 + len2 + 1]; // +1 for the null terminator
       strcpy(result, str1);
       strcat(result, str2);
       return result;
       }


    const char* default_uniforms = R"(

    #version 460 core
    uniform mat4 u_projectionMatrix;
    uniform mat4 u_viewMatrix;
    uniform mat4 u_modelMatrix;
    in vec3 a_position;
    in vec3 a_normal;
    in vec2 a_uv;
    
    )";
    
    
    const char* vsb =  + R"(
 
    uniform sampler2D u_displacementMap;
    uniform vec2 u_repeatAll;
    uniform float u_displacementBias;
    uniform float u_displacementScale;
    out vec3 a_normal_out;
    out vec2 a_uv_out;
    out vec2 a_uvAlpha_out;
    void main()
    {   
       vec2 scaledUV = a_uv * u_repeatAll;
       float displacement = texture(u_displacementMap,scaledUV).r; // displacement from red channel
       displacement *= u_displacementScale + u_displacementBias; // intensity
       vec3 displacedPosition = a_position +  normalize(a_normal) * displacement;
       
       vec4 position = vec4(displacedPosition,1.0f);
       vec4 modelPosition = u_modelMatrix * position;
       vec4 viewPosition  = u_viewMatrix * modelPosition;
       vec4 projectionPosition = u_projectionMatrix * viewPosition;
       gl_Position = projectionPosition;
       
       // size Attenuation
       gl_PointSize = 1.0f;
       gl_PointSize *= (1.0f/-viewPosition.z);

      
       // varying
       a_normal_out = a_normal;
       a_uvAlpha_out = a_uv;
       a_uv_out = scaledUV;


    }
    )";
const char* vs = combine(default_uniforms,vsb);

const char* fs = R"(
#version 460 core

uniform sampler2D u_alphaMap;
uniform sampler2D u_colorMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_aoMap;
uniform sampler2D u_roughnessMap;
uniform sampler2D u_metalnessMap;
uniform sampler2D u_displacementMap;

uniform int isAlphaMap;
uniform int isColorMap;


in vec3 a_normal_out;
in vec2 a_uv_out;
in vec2 a_uvAlpha_out;
out vec4 FragColor;

void main()
{
    // Sample the texture maps
    vec3 baseColor = texture(u_colorMap, a_uv_out).rgb;
    vec3 normal = normalize(texture(u_normalMap, a_uv_out).rgb * 2.0 - 1.0); // Normal map conversion
    float ao = texture(u_aoMap, a_uv_out).r;          // Ambient Occlusion map (red channel)
    float roughness = texture(u_roughnessMap, a_uv_out).r; // Roughness map (red channel)
    float metalness = texture(u_metalnessMap, a_uv_out).r; // Metalness map (red channel)
    float alpha = texture(u_alphaMap, a_uvAlpha_out).g;    // Alpha map (red channel)

    // Combine the texture maps
    vec3 combinedColor = baseColor.rgb * ao; // Basic combination of color and ambient occlusion

    // Roughness and metalness impact the appearance of the material
    // In a more advanced PBR model, these would be used to affect the specular reflection and shading.
    // For simplicity, we'll blend these into the final color.

    // For a more realistic look, you would modify specular highlights or use these values in a PBR model.
    vec3 metallicColor = mix(combinedColor, vec3(1.0), metalness);  // Blend color with white for metallic surfaces
    vec3 finalColor = mix(metallicColor, combinedColor, roughness); // Apply roughness to blend with smoothness

    // Output the final color with the alpha value
    if(isAlphaMap>0)
    {
       FragColor = vec4(finalColor, alpha);  // Correct alpha usage
    }
    else if(isColorMap>0)
    {
       FragColor = vec4(baseColor,1.0);  // Correct alpha usage
    }
    else 
    {
       FragColor = vec4(1.0f,0.0f,1.0f,1.0f);
    }

}
)";
};

       //The commented code block you provided is outlining a typical process in a fragment shader for a 3D rendering pipeline. Here's a breakdown of what each part is doing:
       