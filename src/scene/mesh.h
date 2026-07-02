#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "defines.h"
#include "render/resource/buffer.h"
#include "render/base/device.h"

#include <vector>

namespace mesh {

class Mesh {
public:
    void init(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE const std::vector<Vertex>& vertices() const {
        return m_vertices;
    }
    SV_FORCE_INLINE std::vector<Vertex>& vertices() {
        return m_vertices;
    }

    SV_FORCE_INLINE const std::vector<u32>& indices() const {
        return m_indices;
    }
    SV_FORCE_INLINE std::vector<u32>& indices() {
        return m_indices;
    }

    SV_FORCE_INLINE const std::vector<glm::vec3>& positions() const {
        return m_positions;
    }
    SV_FORCE_INLINE std::vector<glm::vec3>& positions() {
        return m_positions;
    }

    SV_FORCE_INLINE const std::vector<glm::vec2>& texcoords() const {
        return m_texcoords;
    }
    SV_FORCE_INLINE std::vector<glm::vec2>& texcoords() {
        return m_texcoords;
    }

    SV_FORCE_INLINE const std::vector<glm::vec3>& normals() const {
        return m_normals;
    }
    SV_FORCE_INLINE std::vector<glm::vec3>& normals() {
        return m_normals;
    }

    SV_FORCE_INLINE const Buffer& vertexBuffer() const {
        return m_vertexBuffer;
    }
    SV_FORCE_INLINE Buffer& vertexBuffer() {
        return m_vertexBuffer;
    }

    SV_FORCE_INLINE const Buffer& indexBuffer() const {
        return m_indexBuffer;
    }
    SV_FORCE_INLINE Buffer& indexBuffer() {
        return m_indexBuffer;
    }

private:
    std::vector<Vertex> m_vertices;
    std::vector<u32> m_indices;
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec2> m_texcoords;
    std::vector<glm::vec3> m_normals;

    Buffer m_vertexBuffer;
    Buffer m_indexBuffer;
};

} // namespace

using Mesh = mesh::Mesh;
