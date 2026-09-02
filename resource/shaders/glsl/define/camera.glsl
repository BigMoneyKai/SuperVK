#ifndef CAMERA_GLSL
#define CAMERA_GLSL

struct Camera {
    mat4 view;
    mat4 proj;
    vec3 pos;
};

layout(binding = 0) uniform CameraUBO {
    Camera camera;
};

#endif
