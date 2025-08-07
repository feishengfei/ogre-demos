cmake_minimum_required(VERSION 3.10)
project(ogre-demos VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wno-deprecated-declarations)
set(CMAKE_VERBOSE_MAKEFILE ON)

# pkg-config to locate .pc
find_package(PkgConfig REQUIRED)
pkg_check_modules(OGRE REQUIRED OGRE OGRE-Bites OGRE-RTShaderSystem OGRE-Overlay)

message(STATUS "Project version: ${PROJECT_VERSION}")
message(STATUS "Using OGRE version: ${OGRE_VERSION}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_STRIP_COMMAND "strip")
    set(CMAKE_STRIP "strip")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
endif()

set(SOURCE_FILES 
    src/listing4-3__creating_a_log_manager.cpp
    src/listing4-4__intercepting_ogre_logging.cpp
    src/listing4-5__create_render_window.cpp
    src/listing4-5__create_render_window__closable.cpp
    src/listing4-6__load_mesh_demo.cpp
)

macro(create_target target_name source_file)
    add_executable(${target_name} ${source_file})
    target_include_directories(${target_name} PRIVATE ${OGRE_INCLUDE_DIRS} ${OGRE_Bites_INCLUDES_DIRS} ${OGRE_RTShaderSystem_INCLUDES_DIRS} ${OGRE_Overlay_INCLUDES_DIRS})
    target_link_libraries(${target_name} PRIVATE ${OGRE_LIBRARIES})
    target_link_libraries(${target_name} PRIVATE ${OGRE_Bites_LIBRARIES})
    target_link_libraries(${target_name} PRIVATE ${OGRE_RTShaderSystem_LIBRARIES})
    target_link_libraries(${target_name} PRIVATE ${OGRE_Overlay_LIBRARIES})
    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    install(TARGETS ${target_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${target_name}>
            COMMENT "Stripping ${target_name}"
        )
    endif()
endmacro()

foreach(source_file ${SOURCE_FILES})
    get_filename_component(target_name ${source_file} NAME_WE)  #extract filename Without Ending
    create_target(${target_name} ${source_file})
endforeach()
