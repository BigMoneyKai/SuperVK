#pragma once

#include "core/container/array/array.hpp"
#include "core/define/compiler.h"
#include "core/define/types.h"
#include "core/define/vertex.h"

#include "render/resource/buffer.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace mesh {

class Mesh {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice);
  void destroy();

  SV_FORCE_INLINE const Array<Vertex> &vertices() const { return m_vertices; }
  SV_FORCE_INLINE Array<Vertex> &vertices() { return m_vertices; }

  SV_FORCE_INLINE const Array<u32> &indices() const { return m_indices; }
  SV_FORCE_INLINE Array<u32> &indices() { return m_indices; }

  SV_FORCE_INLINE const Array<glm::vec3> &positions() const {
    return m_positions;
  }
  SV_FORCE_INLINE Array<glm::vec3> &positions() { return m_positions; }

  SV_FORCE_INLINE const Array<glm::vec2> &texcoords() const {
    return m_texcoords;
  }
  SV_FORCE_INLINE Array<glm::vec2> &texcoords() { return m_texcoords; }

  SV_FORCE_INLINE const Array<glm::vec3> &normals() const { return m_normals; }
  SV_FORCE_INLINE Array<glm::vec3> &normals() { return m_normals; }

  SV_FORCE_INLINE const Buffer &vertexBuffer() const { return m_vertexBuffer; }
  SV_FORCE_INLINE Buffer &vertexBuffer() { return m_vertexBuffer; }

  SV_FORCE_INLINE const Buffer &indexBuffer() const { return m_indexBuffer; }
  SV_FORCE_INLINE Buffer &indexBuffer() { return m_indexBuffer; }

private:
  Array<Vertex> m_vertices;
  Array<u32> m_indices;
  Array<glm::vec3> m_positions;
  Array<glm::vec2> m_texcoords;
  Array<glm::vec3> m_normals;

  Buffer m_vertexBuffer;
  Buffer m_indexBuffer;
};

} // namespace mesh

using Mesh = mesh::Mesh;
