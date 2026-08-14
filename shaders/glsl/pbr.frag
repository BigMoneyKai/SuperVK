#version 450
#extension GL_GOOGLE_include_directive : require

#include "define/light.glsl"
#include "light/light_pbr.glsl"
#include "define/camera.glsl"
#include "define/material.glsl"
#include "define/texture_sampler.glsl"

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(camera.pos - fragPos);

    vec3 albedo    = material.color;
    float metallic = material.metallic;
    float roughness = material.roughness;

    vec4 texColor = texture(texSampler, fragUV);
    albedo *= texColor.rgb;

    vec3 color = calcPBRAmbient(albedo);
    color += calcDirectionalLightPBR(sunlight, N, V, albedo, metallic, roughness);

    for (int i = 0; i < 1; i++) {
        color += calcPointLightPBR(blobLights[i], N, V, fragPos,
                                   albedo, metallic, roughness);
        color += calcSpotLightPBR(flashLights[i], N, V, fragPos,
                                  albedo, metallic, roughness);
    }

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0);
}
