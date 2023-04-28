cmake_minimum_required(VERSION 3.23)
project(cppSweeper)

set(CMAKE_CXX_STANDARD 17)

add_executable(cppSweeper
        src/main.cpp
        src/Exception/Exception.cpp
        src/Exception/Exception.h
        src/ResourceManager/ResourceManager.cpp
        src/ResourceManager/ResourceManager.h
        src/ResourceManager/stb_image.h
        src/RenderEngine/ShaderProgram.cpp
        src/RenderEngine/ShaderProgram.h
        src/RenderEngine/Texture2D.cpp
        src/RenderEngine/Texture2D.h
        src/RenderEngine/Sprite.cpp
        src/RenderEngine/Sprite.h
        src/Game/Box.cpp
        src/Game/Box.h
        src/Game/GameMap.cpp
        src/Game/GameMap.h
        src/Game/Ranges.cpp
        src/Game/Ranges.h
        src/Game/Bomb.cpp
        src/Game/Bomb.h
        src/Game/Flag.cpp
        src/Game/Flag.h
        src/Game/Game.cpp
        src/Game/Game.h)

# Отключаем опции библиотеки GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

# указываем куда будем класть исполняемый файл
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# копируем папку res к исполняемому файлу
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
