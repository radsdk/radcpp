#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/Integer.h"
#include "rad/Core/Flags.h"
#include "rad/Core/Memory.h"
#include "rad/Core/RefCounted.h"
#include "rad/Core/Span.h"
#include "rad/IO/Logging.h"

#define VK_NO_PROTOTYPES
#include "volk.h"

// Fetch pointers to Vulkan functions dynamically using vkGetInstanceProcAddr, vkGetDeviceProcAddr.
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"
