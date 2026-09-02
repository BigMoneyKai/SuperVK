#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

vec3 calcBlinnAmbient(float ambientStrength, vec3 lightColor) {
    return ambientStrength * lightColor;
}

vec3 calcBlinnDiffuse(vec3 normal, vec3 lightDir, vec3 lightColor) {
    return max(0, dot(normal, lightDir)) * lightColor;
}

vec3 calcBlinnSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor, float shininess) {
    vec3 halfDir = normalize(lightDir + viewDir);
    return pow(max(0, dot(normal, halfDir)), shininess) * lightColor;
}

// for learning
vec3 calcPhongSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor, float shininess) {
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(0, dot(reflectDir, viewDir)), shininess) * lightColor;
}

#endif
