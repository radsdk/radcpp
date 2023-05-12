#pragma once

#include "VulkanConfig.h"

class VulkanObject : public rad::RefCounted<VulkanObject>
{
public:
    VulkanObject() {}
    virtual ~VulkanObject() {}

protected:
    // Disable Copy&Move
    VulkanObject(const VulkanObject&) = delete;
    VulkanObject& operator=(const VulkanObject&) = delete;
    VulkanObject(VulkanObject&&) = delete;
    VulkanObject& operator=(VulkanObject&&) = delete;

}; // class VulkanObject
