# Add source files
file(GLOB_RECURSE SOURCE_FILES 
     *.c
     *.cpp)
    
# Add header files
file(GLOB_RECURSE HEADER_FILES 
     *.h
     *.hpp)

# Copy the entire 'res' directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add shader and texture files
file(GLOB SHADER_FILES
    *.fs
    *.vs
)

file(GLOB MODEL_FILES
    *.png
    *.jpg
    *.obj
    *.mtl
)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES} ${TEXTURE_FILES}
        ../thirdparty/miniaudio.h
        Managers/ComponentsManager.cpp
        Managers/ComponentsManager.h
        Managers/SceneManager.cpp
        Managers/SceneManager.h
        Managers/ResourceManager.cpp
        Managers/ResourceManager.h
        Managers/NodesManager.cpp
        Managers/NodesManager.h Gui/ImguiMain.cpp Gui/ImguiMain.h Gui/ImguiHierarchy.cpp Gui/ImguiHierarchy.h Gui/ImguiInspector.cpp Gui/ImguiInspector.h
        Core/ComponentTypeEnum.h
        HUD/Image/ImageHUD.cpp
        HUD/Image/ImageHUD.h
<<<<<<< Updated upstream
        Managers/GameManager.cpp
        Managers/GameManager.h)
=======
        Component/Animator.cpp
        Component/Animator.h)
>>>>>>> Stashed changes

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                  ${glad_SOURCE_DIR}
                                                  ${stb_image_SOURCE_DIR}
                                                  ${imgui_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} freetype)

target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
