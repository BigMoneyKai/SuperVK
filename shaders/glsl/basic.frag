#version 450
#extension GL_GOOGLE_include_directive : require

#include "include/light.glsl"
#include "include/lighting.glsl"
#include "include/camera.glsl"
#include "include/material.glsl"
#include "include/texture_sampler.glsl"

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    // Lighting
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(camera.pos - fragPos);

    vec3 color = vec3(0.0);
    color += calcAmbient(ambientLight.strength, ambientLight.color) * material.color;
    color += calcDirectionalLight(sunlight, normal, viewDir, material.shininess);
    int pointLightCount = 1;
    for(int i = 0; i < pointLightCount; i++) {
        color += calcPointLight(blobLights[i], normal, viewDir, material.shininess, fragPos);
    }
    int spotLightCount = 1;
    for(int i = 0; i < spotLightCount; i++) {
        color += calcSpotLight(flashLights[i], normal, viewDir, material.shininess, fragPos);
    }

    // texColor
    vec4 texColor = texture(texSampler, fragUV);

    outColor = vec4(color, 1.0) * texColor;
}
