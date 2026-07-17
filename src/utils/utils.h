#pragma once

#include "defines.h"
#include "core/debug/debugger.h"

#include <vulkan/vulkan.h>
#include <stdlib.h>

SV_INLINE void success_exit_impl() {
    exit(EXIT_SUCCESS);
}

SV_INLINE void failure_exit_impl() {
    exit(EXIT_FAILURE);
}

SV_INLINE void except_exit_impl() {

}

#define success_exit()\
    do {\
        TRACE("Exit success");\
        success_exit_impl();\
    } while(0)

#define failure_exit()\
    do {\
        TRACE("Exit failure");\
        failure_exit_impl();\
    } while(0)

#define except_exit(code)\
    do {\
        TRACE("Exit exception: %d", code);\
        except_exit_impl(code);\
    } while(0)

u32 findMemoryType(
    VkPhysicalDevice physicalDevice,
    uint32_t typeFilter,
    VkMemoryPropertyFlags propertiesFlags
);
VkCommandBuffer beginSingleTimeCommands(
    VkDevice device,
    VkCommandPool commandPool
);
void endSingleTimeCommands(
    VkDevice device,
    VkQueue queue,
    VkCommandPool commandPool,
    VkCommandBuffer commandBuffer
);

inline u64 align_up(u64 value, u64 alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

