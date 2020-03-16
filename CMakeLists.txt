# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.8)
project(Aetna)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/glad/include
                    ${CMAKE_SOURCE_DIR}/dependencies/GLFW/include
                    ${CMAKE_SOURCE_DIR}/dependencies/glm/include
                    ${CMAKE_SOURCE_DIR}/dependencies/SOIL/include
                    ${CMAKE_SOURCE_DIR}/dependencies/pugixml/include)
                    
add_library(glfw3 SHARED IMPORTED)
set_property(TARGET glfw3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/dependencies/GLFW/include")
if (MSVC)
	set_property(TARGET glfw3 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/vs2019/glfw3.dll")
	set_property(TARGET glfw3 PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/vs2019/glfw3dll.lib")
	configure_file(${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/vs2019/glfw3.dll ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll COPYONLY)
elseif(MINGW)
	set_property(TARGET glfw3 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/mingw/glfw3.dll")
	set_property(TARGET glfw3 PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/mingw/libglfw3dll.a")
	configure_file(${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/mingw/glfw3.dll ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll COPYONLY)
endif()

add_library(SOIL STATIC IMPORTED)
set_property(TARGET SOIL PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/dependencies/SOIL/include")
set_property(TARGET SOIL PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/dependencies/SOIL/lib/SOIL.lib")

add_executable(Aetna
        dependencies/glad/src/glad.c
        dependencies/pugixml/src/pugixml.cpp
        main.cpp
        core/Material.h
        core/Material.cpp
        core/Mesh.h
        core/Mesh.cpp
        core/Entity.h
        core/Entity.cpp
        core/Object.h
        core/Object.cpp
        core/Camera.h
        core/Camera.cpp
        core/framework/render/Renderer.h
        core/framework/render/OpenGLRenderer.cpp
        core/framework/render/OpenGLRenderer.h
        core/framework/Engine.cpp
        core/framework/Engine.h 
        core/framework/Controls.cpp
        core/framework/Controls.h 
        core/framework/Lights.h 
		core/framework/Script.h 
		MainScript.cpp 
		MainScript.h 
		core/framework/Game.cpp 
		core/framework/Game.h
        core/framework/shader/Shader.h
        core/framework/shader/OpenGLShader.h
        core/framework/shader/OpenGLShader.cpp
		core/framework/shader/ShaderProgram.h
		core/framework/shader/OpenGLShaderProgram.cpp 
		core/framework/shader/OpenGLShaderProgram.h
        core/framework/Utils.cpp
        core/framework/Utils.h
        core/framework/Materials.h
        core/framework/input/InputProxy.h
        core/framework/input/InputProxy.cpp
        core/framework/input/InputGLFW.h
        core/framework/input/InputGLFW.cpp
        core/framework/objects/Player.h
        core/framework/objects/PlayerFree.h
        core/framework/objects/PlayerFree.cpp)

target_link_libraries(Aetna glfw3)