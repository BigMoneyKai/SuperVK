#ifndef LIGHT_BLINN_GLSL
#define LIGHT_BLINN_GLSL

#include "define/light.glsl"
#include "blinn_phong/lighting_blinn_phong.glsl"

vec3 calcAmbient(float ambientStrength, vec3 lightColor) {
    return ambientStrength * lightColor;
}

float calcAttenuation(float constant, float linear, float quadratic, float distance) {
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

vec3 calcDirectionalLight(DirectionalLight dl, vec3 normal, vec3 viewDir, float shininess) {
    vec3 result = vec3(0.0);
    result += calcBlinnDiffuse(normal, dl.dir, dl.color);
    result += calcBlinnSpecular(normal, dl.dir, viewDir, dl.color, shininess);
    return result;
}

vec3 calcPointLight(PointLight pl, vec3 normal, vec3 viewDir, float shininess, vec3 fragPos) {
    vec3 lightDir = normalize(pl.pos - fragPos);
    vec3 diffuse = calcBlinnDiffuse(normal, lightDir, pl.color);
    vec3 specular = calcBlinnSpecular(normal, lightDir, viewDir, pl.color, shininess);
    float attenuation = calcAttenuation(pl.constant, pl.linear, pl.quadratic, distance(pl.pos, fragPos));
    vec3 result = (diffuse + specular) * attenuation;
    return result;
}

vec3 calcSpotLight(SpotLight sl, vec3 normal, vec3 viewDir, float shininess, vec3 fragPos) {
    vec3 lightDir = normalize(sl.pos - fragPos);
    vec3 diffuse = calcBlinnDiffuse(normal, lightDir, sl.color);
    vec3 specular = calcBlinnSpecular(normal, lightDir, viewDir, sl.color, shininess);
    float attenuation = calcAttenuation(sl.constant, sl.linear, sl.quadratic, distance(sl.pos, fragPos));
    float epsilon = sl.innerCutoff - sl.outerCutoff;
    float theta = dot(lightDir, normalize(-sl.dir));
    float intensity = sl.intensity * clamp((theta - sl.outerCutoff) / epsilon, 0.0, 1.0);
    vec3 result = (diffuse + specular) * attenuation * intensity;
    return result;
}

#endif
