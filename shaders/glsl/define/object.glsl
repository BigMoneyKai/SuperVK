#ifndef OBJECT_GLSL
#define OBJECT_GLSL

struct Object {
    mat4 model;
};

layout(binding = 1) uniform ObjectUBO {
    Object object;
};


#endif
