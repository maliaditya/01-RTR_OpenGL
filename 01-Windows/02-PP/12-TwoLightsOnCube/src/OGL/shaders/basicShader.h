struct BasicShader
{

    
    
const char* vs =  + R"(
#version 460 core

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform sampler2D u_displacementMap;
uniform float u_displacementScale;
uniform float u_displacementBias;
uniform vec2 u_repeatAll;

out vec3 a_normal_out;
out vec2 a_uv_out;
out vec2 a_uvAlpha_out;

void main()
{   
   vec2 scaledUV = a_uv * u_repeatAll; // tiles
   vec4 position;
   if(u_displacementBias>0.0f)
   {
      float displacement = texture(u_displacementMap,scaledUV).r; // displacement from red channel
      displacement *= 0.3 + u_displacementBias; // intensity
      vec3 displacedPosition = a_position +  normalize(a_normal) * displacement;
      position = vec4(displacedPosition,1.0f);
   }
   else
   {
      position = vec4(a_position,1.0f);
   }
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
    // Output the final color with the alpha value
    if(isAlphaMap>0)
    { 
      vec3 baseColor = texture(u_colorMap, a_uv_out).rgb;
      float alpha = texture(u_alphaMap, a_uvAlpha_out).g;    
      FragColor = vec4( baseColor , alpha); 
   }
    else if(isColorMap>0)
    {
      vec3 baseColor = texture(u_colorMap, a_uv_out).rgb;
      FragColor = vec4(baseColor,1.0); 
    }
    else 
    {
       FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
    }
}
)";
};

       //The commented code block you provided is outlining a typical process in a fragment shader for a 3D rendering pipeline. Here's a breakdown of what each part is doing:
       