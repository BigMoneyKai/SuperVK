#pragma once

#include <glm/glm.hpp>

namespace vertex {

constexpr glm::vec3 defaultPos = {0.f, 0.f, 0.f};
constexpr glm::vec3 defaultColor = {0.f, 0.f, 0.f};
constexpr glm::vec2 defaultUV = {0.f, 0.f};
constexpr glm::vec3 defaultNormal = {0.f, 0.f, 0.f};

// Data type struct
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
    glm::vec3 normal;
};

} // namespace

using Vertex = vertex::Vertex;
