# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.30)
project(ITD102_GB_Emulator C)

set(CMAKE_C_STANDARD 23)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")

# ----- DEFINE EXECUTABLE ----- #
add_executable(ITD102_GB_Emulator ${SRC_DIR}/main.c)

# ----- IMPORT GLFW ----- #
set(GLFW_DIR "${LIB_DIR}/glfw")
add_subdirectory("${GLFW_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include/GLFW")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")

# ----- IMPORT GLAD ----- #
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# ----- Link Libraries ----- #
target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARIES}
        GL      # OpenGL
        m       # Math
        dl      # Dynamic loader
        pthread # Threads
        X11     # X11 window system
)