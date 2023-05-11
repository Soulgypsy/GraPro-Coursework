#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 1) in vec2 TextCoords;

out Vertex_DATA{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
    vec3 vFogPosition;
} vs_Out;
out vec3 vFogPosition;


uniform mat4 model;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_Out.tC = TextCoords;
    vs_Out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_Out.Position = vec3(model * vec4(aPos, 1.0));
     
    vec4 worldPosition = model * vec4(aPos, 1.0);
    vs_Out.vFogPosition = vec3(view * worldPosition);
    gl_Position = transform * vec4(aPos, 1.0);
}  
