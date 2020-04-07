cmake_minimum_required(VERSION 3.8)
project(Aetna)

set(CMAKE_CXX_STANDARD 17)

file(READ "${CMAKE_SOURCE_DIR}/../project.aetna" PROJECT_FILE)

string(REGEX MATCH "\"core\"[ :]+\"([^\\\"]*)\"" _ ${PROJECT_FILE})
set(JSON_CORE_PATH ${CMAKE_MATCH_1})

string(REGEX MATCH "\"project\"[ :]+[\\\{\\\n]([^}]*\\\})" _ ${PROJECT_FILE})
set(JSON_PROJECT ${CMAKE_MATCH_1})
string(REGEX MATCH "\"name\"[ :]+\"([^\\\"]*)\"" _ ${JSON_PROJECT})
set(AETNA_PROJECT_NAME ${CMAKE_MATCH_1})

set(ENGINE_PATH ${JSON_CORE_PATH}/source)
add_definitions(-DENGINE_PATH="${ENGINE_PATH}")
add_definitions(-DAETNA_PROJECT_NAME="${AETNA_PROJECT_NAME}")

add_subdirectory(${ENGINE_PATH} "${CMAKE_CURRENT_BINARY_DIR}/core_build")

include_directories("${ENGINE_PATH}/../dependencies/glm/include"
                    "${ENGINE_PATH}/../dependencies/pugixml/include"
                    "${ENGINE_PATH}/../dependencies/stb_image/include"
					"${ENGINE_PATH}"
					)

add_executable(Aetna
        main.cpp
		MainScript.cpp 
		MainScript.h 
)

target_link_libraries(Aetna AetnaCore)