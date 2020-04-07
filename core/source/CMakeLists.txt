cmake_minimum_required(VERSION 3.8)
project(AetnaCore)

include_directories("${ENGINE_PATH}/../dependencies/glad/include"
                    "${ENGINE_PATH}/../dependencies/GLFW/include"
                    "${ENGINE_PATH}/../dependencies/glm/include"
                    "${ENGINE_PATH}/../dependencies/SOIL/include"
                    "${ENGINE_PATH}/../dependencies/pugixml/include"
                    "${ENGINE_PATH}/../dependencies/stb_image/include"
					"${ENGINE_PATH}"
					)

add_library(glfw3 SHARED IMPORTED)
set_property(TARGET glfw3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${ENGINE_PATH}/../dependencies/GLFW/include")
if (MSVC)
	set_property(TARGET glfw3 PROPERTY IMPORTED_LOCATION "${ENGINE_PATH}/../dependencies/GLFW/lib/vs2019/glfw3.dll")
	set_property(TARGET glfw3 PROPERTY IMPORTED_IMPLIB "${ENGINE_PATH}/../dependencies/GLFW/lib/vs2019/glfw3dll.lib")
	configure_file(${ENGINE_PATH}/../dependencies/GLFW/lib/vs2019/glfw3.dll ${CMAKE_CURRENT_BINARY_DIR}/../glfw3.dll COPYONLY)
elseif(MINGW)
	set_property(TARGET glfw3 PROPERTY IMPORTED_LOCATION "${ENGINE_PATH}/../dependencies/GLFW/lib/mingw/glfw3.dll")
	set_property(TARGET glfw3 PROPERTY IMPORTED_IMPLIB "${ENGINE_PATH}/../dependencies/GLFW/lib/mingw/libglfw3dll.a")
	configure_file(${ENGINE_PATH}/../dependencies/GLFW/lib/mingw/glfw3.dll ${CMAKE_CURRENT_BINARY_DIR}/../glfw3.dll COPYONLY)
endif()

add_library(SOIL STATIC IMPORTED)
set_property(TARGET SOIL PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${ENGINE_PATH}/../dependencies/SOIL/include")
set_property(TARGET SOIL PROPERTY IMPORTED_LOCATION "${ENGINE_PATH}/../dependencies/SOIL/lib/SOIL.lib")

set(AETNA_CORE_HEADERS
	Material.h
	Mesh.h
	Texture.h
	Utils.h
	entities/Entity.h
	entities/Lights.h
	entities/objects/Object.h
	entities/objects/Player.h
	entities/objects/PlayerFree.h
	framework/Application.h
	framework/ApplicationGL.h
	framework/Camera.h
	framework/Engine.h
	framework/Game.h
	framework/Materials.h
	framework/Script.h
	framework/input/Controls.h
	framework/input/raw/InputGLFW.h
	framework/input/raw/InputProxy.h
	framework/render/OpenGLRenderer.h
	framework/render/Renderer.h
	framework/render/shader/OpenGLShader.h
	framework/render/shader/OpenGLShaderProgram.h
	framework/render/shader/Shader.h
	framework/render/shader/ShaderProgram.h
)

set(AETNA_CORE_SOURCE
	Material.cpp
	Mesh.cpp
	Texture.cpp
	Utils.cpp
	entities/Entity.cpp
	entities/objects/Object.cpp
	entities/objects/PlayerFree.cpp
	framework/Application.cpp
	framework/ApplicationGL.cpp
	framework/Camera.cpp
	framework/Engine.cpp
	framework/Game.cpp
	framework/input/Controls.cpp
	framework/input/raw/InputGLFW.cpp
	framework/input/raw/InputProxy.cpp
	framework/render/OpenGLRenderer.cpp
	framework/render/shader/OpenGLShader.cpp
	framework/render/shader/OpenGLShaderProgram.cpp
)

add_library(AetnaCore
	../dependencies/glad/src/glad.c
	../dependencies/stb_image/include/stb_image.h
	../dependencies/pugixml/src/pugixml.cpp
	${AETNA_CORE_HEADERS}
	${AETNA_CORE_SOURCE}
)

target_link_libraries(AetnaCore
    glfw3
    SOIL
)
