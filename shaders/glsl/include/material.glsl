#ifndef MATERIAL_GLSL
#define MATERIAL_GLSL

struct Material {
    vec3 color;
    float metallic;
    float roughness;
    float shininess;
};

layout(binding = 3) uniform MaterialUBO {
    Material material;
};

#endif
