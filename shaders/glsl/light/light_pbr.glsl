#ifndef LIGHT_PBR_GLSL
#define LIGHT_PBR_GLSL

#include "define/light.glsl"
#include "pbr/lighting_pbr.glsl"

float calcAttenuation(float constant, float linear, float quadratic, float dist) {
    return 1.0 / (constant + linear * dist + quadratic * dist * dist);
}

vec3 calcDirectionalLightPBR(DirectionalLight dl, vec3 N, vec3 V,
                              vec3 albedo, float metallic, float roughness) {
    vec3 L = normalize(-dl.dir);
    float NdotL = max(dot(N, L), 0.0);
    return calcPBR(N, V, L, albedo, metallic, roughness, dl.color, NdotL);
}

vec3 calcPointLightPBR(PointLight pl, vec3 N, vec3 V, vec3 fragPos,
                        vec3 albedo, float metallic, float roughness) {
    vec3 L = normalize(pl.pos - fragPos);
    float NdotL = max(dot(N, L), 0.0);
    float dist = distance(pl.pos, fragPos);
    float attenuation = calcAttenuation(pl.constant, pl.linear, pl.quadratic, dist);
    return calcPBR(N, V, L, albedo, metallic, roughness, pl.color, NdotL) * attenuation;
}

vec3 calcSpotLightPBR(SpotLight sl, vec3 N, vec3 V, vec3 fragPos,
                       vec3 albedo, float metallic, float roughness) {
    vec3 L = normalize(sl.pos - fragPos);
    float NdotL = max(dot(N, L), 0.0);
    float dist = distance(sl.pos, fragPos);
    float attenuation = calcAttenuation(sl.constant, sl.linear, sl.quadratic, dist);
    float theta = dot(L, normalize(-sl.dir));
    float epsilon = sl.innerCutoff - sl.outerCutoff;
    float intensity = clamp((theta - sl.outerCutoff) / epsilon, 0.0, 1.0);
    return calcPBR(N, V, L, albedo, metallic, roughness, sl.color, NdotL)
           * attenuation * intensity;
}

#endif
