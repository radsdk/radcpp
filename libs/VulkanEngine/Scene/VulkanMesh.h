#pragma once

#include "VulkanEngine/VulkanCore.h"
#include "rad/Math/3DMath.h"

class VulkanMesh
{
public:
    VulkanMesh();
    VulkanMesh(std::string name);
    ~VulkanMesh();

    std::string m_name;
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_tangents;
    std::vector<glm::vec3> m_bitangents;
    std::vector<std::vector<glm::vec4>> m_colorChannels;
    std::vector<std::string> m_texCoordChannelNames;
    std::vector<std::vector<glm::vec2>> m_texCoordChannels;

    struct Polygon
    {
        std::vector<uint32_t> indices;
    };
    std::vector<Polygon> m_polygons;

    enum class PrimitiveType
    {
        Point = 0x1,
        Line = 0x2,
        Triangle = 0x4,
        Polygon = 0x8,
    };
    rad::Flags32<PrimitiveType> m_primitiveTypes;
    bool IsTriangleMesh() { return (m_primitiveTypes == PrimitiveType::Triangle); }

    rad::BoundingBox<float> m_boundingBox;

    struct VertexWeight
    {
        uint32_t vertexIndex;
        float weight;
    };

    struct Bone
    {
        std::string name;
        std::vector<VertexWeight> weights;
        // Matrix that transforms from mesh space to bone space in bind pose.
        glm::mat4 inverseBindMatrix;
    };
    std::vector<Bone> m_bones;

    rad::Ref<VulkanBuffer> m_vertexBuffer;
    VkDeviceSize m_vertexBufferOffset = 0;
    enum VertexAttribute : uint32_t
    {
        VertexAttributePosition,    // vec3
        VertexAttributeNormal,      // vec3
        VertexAttributeTangent,     // vec4
        VertexAttributeColor,       // vec4
        VertexAttributeTexCoord0,   // vec2
        VertexAttributeTexCoord1,   // vec2
        VertexAttributeTexCoord2,   // vec2
        VertexAttributeTexCoord3,   // vec2
    };
    // 1 << VertexAttributeLocation;
    uint32_t m_vertexFormatFlags = 0;
    void SetVertexAttribute(VertexAttribute attr)
    {
        m_vertexFormatFlags |= (1 << attr);
    }
    bool HasVertexAttribute(VertexAttribute attr)
    {
        return (m_vertexFormatFlags & (1 << attr));
    }
    uint32_t m_vertexStride = 0;
    rad::Ref<VulkanBuffer> m_indexBuffer;
    VkDeviceSize m_indexBufferOffset = 0;

}; // class VulkanMesh
