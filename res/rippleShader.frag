#version 400 core

in Vertex_DATA {
    vec2 tC;
    vec3 Normal;
    vec3 Position;
    vec3 vFogPosition;
} vs_In;

out vec4 FragColor;
out vec4 GlitchColor;
out vec4 WaveColor;
out vec4 FogColor;
out vec4 HologramColor;
out vec4 color;

uniform sampler2D texture1;
uniform sampler2D hologramTexture;
uniform float timeFragment;
uniform float intensity;
uniform float fogDensity;
uniform float colorShiftSpeed;
uniform float flickerSpeed;
uniform float transparency;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.0, 78.0))) * 4000.0);
}

void main()
{
  // Creates the ripple effect with a uv
    vec2 uv = vs_In.tC.xy;
    uv.x += sin(uv.y * 10.0 + timeFragment * 5.0) * 0.05;
    uv.y += sin(uv.x * 10.0 + timeFragment * 5.0) * 0.05;
    WaveColor = texture(texture1, uv);

  // Add random displacements to texture coordinates
    vec2 distortedCoords = vs_In.tC;
    distortedCoords.x += intensity * (rand(vec2(vs_In.tC.y, timeFragment)) - 0.5);
    distortedCoords.y += intensity * (rand(vec2(vs_In.tC.x, timeFragment)) - 0.5);

    // Apply color shifting
    vec4 color = vec4(0.0);
    color.r = texture(texture1, distortedCoords + vec2(intensity * sin(timeFragment), 5.0)).r;
    color.g = texture(texture1, distortedCoords + vec2(5.0, intensity * cos(timeFragment))).g;
    color.b = texture(texture1, distortedCoords - vec2(intensity * sin(timeFragment), 5.0)).b;
    color.a = texture(texture1, distortedCoords - vec2(5.0, intensity * cos(timeFragment))).a;
    
    GlitchColor = color;

    // Places the fog effect on the model
    vec3 fogColorFactor = vec3 (2, 2, 5);
    float distance = length(vs_In.vFogPosition);
    float fogFactor = 1.0 - exp(-fogDensity * distance * distance);

    FogColor = vec4(fogColorFactor, fogFactor);

    // Shifts the texture coordinates and creates a hologram effect
    vec2 shiftedTexCoord = vec2(vs_In.tC.x + sin(timeFragment * colorShiftSpeed), vs_In.tC.y);
    vec4 hologramColorTexture = texture(hologramTexture, shiftedTexCoord);
    float flickerFactor = sin(timeFragment * flickerSpeed) * 0.5 + 0.7;
    HologramColor = hologramColorTexture * vec4(flickerFactor, flickerFactor, flickerFactor, transparency);

    FragColor = WaveColor * GlitchColor * FogColor * HologramColor;
}