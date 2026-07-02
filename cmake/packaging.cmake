# cmake/packaging.cmake

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "SuperVK")
set(CPACK_PACKAGE_VENDOR "Kai")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

set(CPACK_PACKAGE_CONTACT "1600268125@qq.com")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "A Vulkan Rendering Engine"
)

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")

elseif(WIN32)
    set(CPACK_GENERATOR "ZIP")

else(UNIX NOT APPLE)
    set(CPACK_GENERATOR "TGZ")

endif()

include(CPack)
