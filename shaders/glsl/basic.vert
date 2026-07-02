#version 450
#extension GL_GOOGLE_include_directive : require

#include "include/lighting.glsl"
#include "include/object.glsl"
#include "include/camera.glsl"

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragUV;
layout(location = 3) out vec3 fragNormal;

void main() {
    gl_Position = camera.proj * camera.view * object.model * vec4(inPos, 1.0);
    vec4 worldPos = object.model * vec4(inPos, 1.0);
    fragPos = worldPos.xyz;
    fragColor = inColor;
    fragUV = inUV;
    fragNormal = normalize(mat3(transpose(inverse(object.model))) * inNormal);
}
