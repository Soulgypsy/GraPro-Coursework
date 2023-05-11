#version 420

out vec4 FragColor;

in Vertex_DATA{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vs_In;

uniform vec3 camPos;
uniform samplerCube skybox;
layout (binding = 1) uniform sampler2D diffuse;

 
void main()
{
    vec3 I = normalize(vs_In.Position - camPos); //vector to camera
    vec3 R = reflect(I, normalize(vs_In.Normal)); //calculate the reflection direction for the incident vector
    vec4 Colour = vec4(texture(skybox, R).rgb, 1.0);

    vec4 textureMix = mix(texture(diffuse,vs_In.tC), Colour, 0.5f); // mix textures to create the texture look

    FragColor = textureMix;
}