add_library(supervk_core_job STATIC)

target_sources(supervk_core_job PRIVATE
    job.cpp
    job_queue.cpp
    job_system.cpp
    worker.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_job PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_job PRIVATE
    supervk_platform
    supervk_core_memory
    supervk_core_container_array
    supervk_core_container_queue
    supervk_debug
    supervk_utils

    Vulkan::Vulkan
)
