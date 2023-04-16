cmake_minimum_required(VERSION 3.9)

project(Renderer)
add_subdirectory(Scene/tinyobjloader)

add_executable(renderer
                Camera/perspective.cpp
                Image/ImagePPM.cpp
                Primitive/BRDF/Phong.cpp
                Primitive/Geometry/mesh.cpp
                Renderer/StandardRenderer.cpp
                Scene/scene.cpp
                Shader/AmbientShader.cpp
                Shader/WhittedShader.cpp
                utils/AABB.cpp
                main.cpp)
target_include_directories(renderer PRIVATE "3DSortingStruct/" "Camera/" "Image/" "Light/" "Primitive/" "Primitive/BRDF" "Primitive/Geometry" "Rays/" "Renderer/" "Scene/" "Shader/" "utils/" )
target_link_libraries(renderer tinyobjloader)