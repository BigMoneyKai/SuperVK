#ifndef LIGHT_GLSL
#define LIGHT_GLSL

#include "include/lighting.glsl"

struct AmbientLight {
    float strength;
    vec3 color;
};

struct DirectionalLight {
    vec3 dir;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 pos;
    vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 pos;
    vec3 dir;
    vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;

    float innerCutoff;
    float outerCutoff;
};

layout(binding = 2) uniform LightUBO {
    AmbientLight ambientLight;
    DirectionalLight sunlight;
    PointLight blobLights[1];
    SpotLight flashLights[1];
};

float calcAttenuation(float constant, float linear, float quadratic, float distance) {
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

vec3 calcDirectionalLight(DirectionalLight dl, vec3 normal, vec3 viewDir, float shininess) {
    vec3 result = vec3(0.0);
    result += calcDiffuse(normal, dl.dir, dl.color);
    result += calcSpecular(normal, dl.dir, viewDir, dl.color, shininess);
    return result;
}
vec3 calcPointLight(PointLight pl, vec3 normal, vec3 viewDir, float shininess, vec3 fragPos) {
    vec3 lightDir = normalize(pl.pos - fragPos);
    vec3 diffuse = calcDiffuse(normal, lightDir, pl.color);
    vec3 specular = calcSpecular(normal, lightDir, viewDir, pl.color, shininess);
    float attenuation = calcAttenuation(pl.constant, pl.linear, pl.quadratic, distance(pl.pos, fragPos));
    vec3 result = (diffuse + specular) * attenuation;
    return result;
}
vec3 calcSpotLight(SpotLight sl, vec3 normal, vec3 viewDir, float shininess, vec3 fragPos) {
    vec3 diffuse = calcDiffuse(normal, sl.dir, sl.color);
    vec3 specular = calcSpecular(normal, sl.dir, viewDir, sl.color, shininess);
    float attenuation = calcAttenuation(sl.constant, sl.linear, sl.quadratic, distance(sl.pos, fragPos));
    float intensity = sl.intensity;
    vec3 result = (diffuse + specular) * attenuation * intensity;
    return result;
}

#endif
