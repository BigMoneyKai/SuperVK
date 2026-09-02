#ifndef LIGHTING_PBR_GLSL
#define LIGHTING_PBR_GLSL

#include "pbr.glsl"
#include "define/common.glsl"

vec3 calcPBR(vec3 N, vec3 V, vec3 L, vec3 albedo, float metallic, float roughness,
             vec3 lightColor, float NdotL) {

    vec3 H = normalize(L + V);
    float NdotV = max(dot(N, V), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    vec3 f0 = mix(vec3(0.04), albedo, metallic);

    vec3  F = fresnelSchlick(HdotV, f0);
    float D = distributionGGX(N, H, roughness);
    float G = geometrySmith(NdotV, NdotL, roughness);

    vec3 specular = (F * D * G) / max(4.0 * NdotV * NdotL, 0.001);

    vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 diffuse = kD * albedo / PI;

    return (diffuse + specular) * lightColor * NdotL;
}

vec3 calcPBRAmbient(vec3 albedo) {
    return vec3(0.03) * albedo;
}

#endif
