#include "debug/debugger.h"
#include "scene/mesh.h"

void Mesh::init(const VkDevice& device, const VkPhysicalDevice& physicalDevice) {
    if(m_vertices.empty() || m_indices.empty()) {
        FATAL("Mesh data is empty");
    }

    m_vertexBuffer.init(
        BT_VERTEX_BUFFER,
        device,
        physicalDevice,
        m_vertices.data(),
        sizeof(Vertex) * m_vertices.size()
    );
    m_indexBuffer.init(
        BT_INDEX_BUFFER,
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
