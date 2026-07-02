#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

vec3 calcAmbient(float ambientStrength, vec3 lightColor) {
    return ambientStrength * lightColor;
}

vec3 calcDiffuse(vec3 normal, vec3 lightDir, vec3 lightColor) {
    return max(0, dot(normal, lightDir)) * lightColor;
}

// for learning
vec3 calcSpecularPhong(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor, float shininess) {
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(0, dot(reflectDir, viewDir)), shininess) * lightColor;
}

vec3 calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor, float shininess) {
    vec3 halfDir = normalize(lightDir + viewDir);
    return pow(max(0, dot(normal, halfDir)), shininess) * lightColor;
}

#endif
