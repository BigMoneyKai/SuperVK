#include "core/debug/debugger.h"
#include "scene/mesh.h"

void Mesh::init(const VkDevice& device, const VkPhysicalDevice& physicalDevice) {
    if(m_vertices.empty() || m_indices.empty()) {
        FATAL(LogCatag::scene, "Mesh data is empty");
    }

    m_vertexBuffer.init(
        BufferType::vertexBuffer,
        device,
        physicalDevice,
        m_vertices.data(),
        sizeof(Vertex) * m_vertices.size()
    );
    m_indexBuffer.init(
        BufferType::indexBuffer,
        device,
        physicalDevice,
        m_indices.data(),
        sizeof(u32) * m_indices.size()
    );
    m_indexBuffer.printDebugInfo();
    m_vertexBuffer.printDebugInfo();

}

void Mesh::destroy() {
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
}
