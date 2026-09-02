#ifndef LIGHT_GLSL
#define LIGHT_GLSL

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

#endif
