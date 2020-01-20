# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.14)
project(Aetna)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

include_directories(dependencies/glad/include dependencies/GLFW/include dependencies/glm dependencies/SOIL/include)
link_directories(dependencies/SOIL/lib dependencies/GLFW/lib)

add_executable(Aetna
        dependencies/glad/src/glad.c
        main.cpp
        core/Mesh.h
        core/Mesh.cpp
        core/Object.h
        core/Object.cpp
        core/framework/render/Renderer.h
        core/framework/render/OpenGLRenderer.cpp
        core/framework/render/OpenGLRenderer.h
        core/framework/Engine.cpp
        core/framework/Engine.h core/framework/Script.h MainScript.cpp MainScript.h core/framework/Game.cpp core/framework/Game.h core/framework/shader/ShaderProgram.h core/framework/shader/OpenGLShaderProgram.cpp core/framework/shader/OpenGLShaderProgram.h)

target_link_libraries(Aetna SOIL ${OPENGL_gl_LIBRARY} glfw3 "-static")