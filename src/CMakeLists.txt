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

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/src)
file(COPY ${CMAKE_SOURCE_DIR}/scenes DESTINATION ${CMAKE_BINARY_DIR}/src)
file(COPY ${CMAKE_SOURCE_DIR}/thirdparty DESTINATION ${CMAKE_BINARY_DIR}/src)

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
        Managers/NodesManager.h
        Core/ComponentTypeEnum.h
        HUD/Image/ImageRenderer.cpp
        HUD/Image/ImageRenderer.h
        Managers/GameManager.cpp
        Managers/GameManager.h
        Managers/AudioManager.cpp
        Managers/AudioManager.h
        Sound.cpp
        Sound.h
        AudioContext.cpp
        Component/PlayerAudioController.cpp
        Component/PlayerAudioController.h
        Enemies/EnemiesManager.cpp
        Enemies/EnemiesManager.h
        Enemies/Enemy.cpp
        Enemies/Enemy.h
        Component/Animation.h Component/ShadowMap.h Component/ShadowMap.cpp Component/CloudManager.cpp
        HUD/Pages/Page.cpp
        HUD/Pages/Page.h
        HUD/PageManager.cpp
        HUD/PageManager.h
        HUD/Pages/PauseMenu/PauseMenuPage.cpp
        HUD/Pages/PauseMenu/PauseMenuPage.h
        HUD/Pages/PlayerUpgradeMenu/PlayerUpgradeMenu.cpp
        HUD/Pages/PlayerUpgradeMenu/PlayerUpgradeMenu.h
        HUD/Buttons/Button.cpp
        HUD/Buttons/Button.h
        Managers/DomeManager.cpp
        Managers/DomeManager.h
        Managers/UpgradeManager.cpp
        Managers/UpgradeManager.h
        Turrets/TurretsManager.cpp
        Turrets/TurretsManager.h
        Turrets/Turret.cpp
        Turrets/Turret.h
	  ComputeShader.h
        HUD/Pages/TurretUpgradeMenu/TurretUpgradeMenu.cpp
        HUD/Pages/TurretUpgradeMenu/TurretUpgradeMenu.h
        HUD/Pages/DomeUpgradeMenu/DomeUpgradeMenu.cpp
        HUD/Pages/DomeUpgradeMenu/DomeUpgradeMenu.h
        Component/RefuelStation.cpp
        Component/RefuelStation.h
        HUD/Pages/MessagePage/MessagePage.cpp
        HUD/Pages/MessagePage/MessagePage.h
        Managers/TutorialManager.cpp
        Managers/TutorialManager.h
        Component/ShovelController.cpp
        Component/ShovelController.h
        Component/ShovelRenderer.cpp
        Component/ShovelRenderer.h
        Component/Rotate.h
        model.h
        mesh.h
        shader_s.h
        Component/Rotate.cpp
        Component/Disabler.h
        Component/Disabler.cpp
        Managers/LightsManager.h
        Managers/LightsManager.cpp
        Component/GlowstickMovement.h
        Component/GlowstickMovement.cpp
        Component/PDAController.cpp
        Component/PDAController.h
        HUD/Pages/PDA/PDAPage.cpp
        HUD/Pages/PDA/PDAPage.h
        WindSimulation.h
        WindSimulation.cpp
        HUD/Pages/MainMenuPage/MainMenuPage.cpp
        HUD/Pages/MainMenuPage/MainMenuPage.h
        Managers/WeatherManager.h 
	Managers/WeatherManager.cpp 
	Framebuffer.h 
	Framebuffer.cpp
		HUD/CoordsConverter.cpp
		HUD/CoordsConverter.h Component/CompassController.h Component/CompassController.cpp
		HUD/Pages/RestartPage/RestartPage.cpp
		HUD/Pages/RestartPage/RestartPage.h
		Managers/TransparentRenderer.cpp
		HUD/Pages/Settings/SettingsPage.cpp
        HUD/Pages/Settings/SettingsPage.h
		HUD/Pages/Controls/ControlsPage.cpp
		HUD/Pages/Controls/ControlsPage.h
		HUD/Pages/CreditsPage/CreditsPage.cpp
		HUD/Pages/CreditsPage/CreditsPage.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                  ${glad_SOURCE_DIR}
                                                  ${stb_image_SOURCE_DIR}
                                                  )

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} freetype)

target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
