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
        Managers/NodesManager.h Gui/ImguiMain.cpp Gui/ImguiMain.h Gui/ImguiHierarchy.cpp Gui/ImguiHierarchy.h Gui/ImguiInspector.cpp Gui/ImguiInspector.h
        Core/ComponentTypeEnum.h
        HUD/Image/ImageRenderer.cpp
        HUD/Image/ImageRenderer.h
        Managers/GameManager.cpp
        Managers/GameManager.h
        Component/Animator.cpp
        Component/Animator.h
        Managers/AudioEngineManager.cpp
        Managers/AudioEngineManager.h
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
        HUD/Buttons/PauseMenu/ResumeButton.cpp
        HUD/Buttons/PauseMenu/ResumeButton.h
        HUD/Buttons/PauseMenu/QuitButton.cpp
        HUD/Buttons/PauseMenu/QuitButton.h
        HUD/Buttons/PauseMenu/QuitButton.cpp
        HUD/Buttons/PauseMenu/SettingsButton.cpp
        HUD/Buttons/PauseMenu/SettingsButton.h
        Managers/DomeManager.cpp
        Managers/DomeManager.h
        Managers/UpgradeManager.cpp
        Managers/UpgradeManager.h
        Turrets/TurretsManager.cpp
        Turrets/TurretsManager.h
        Turrets/Turret.cpp
        Turrets/Turret.h
	  ComputeShader.h
        HUD/Buttons/Player/UpgradeReach.cpp
        HUD/Buttons/Player/UpgradeReach.h
        HUD/Buttons/Player/UpgradeRadius.cpp
        HUD/Buttons/Player/UpgradeRadius.h
        HUD/Buttons/Player/UpgradeSpeed.cpp
        HUD/Buttons/Player/UpgradeSpeed.h
        HUD/Pages/TurretUpgradeMenu/TurretUpgradeMenu.cpp
        HUD/Pages/TurretUpgradeMenu/TurretUpgradeMenu.h
        HUD/Buttons/TurretUpgrades/UpgradeFireRate.cpp
        HUD/Buttons/TurretUpgrades/UpgradeFireRate.h
        HUD/Buttons/TurretUpgrades/UpgradeDamage.cpp
        HUD/Buttons/TurretUpgrades/UpgradeDamage.h
        HUD/Pages/DomeUpgradeMenu/DomeUpgradeMenu.cpp
        HUD/Pages/DomeUpgradeMenu/DomeUpgradeMenu.h
        HUD/Buttons/DomeUpgrades/UpgradeDomeHP.cpp
        HUD/Buttons/DomeUpgrades/UpgradeDomeHP.h
        HUD/Buttons/Player/JetpackRefuel.cpp
        HUD/Buttons/Player/JetpackRefuel.h
        Component/RefuelStation.cpp
        Component/RefuelStation.h
        HUD/Pages/MessagePage/MessagePage.cpp
        HUD/Pages/MessagePage/MessagePage.h
        Managers/TutorialManager.cpp
        Managers/TutorialManager.h
        HUD/Buttons/MessagePage/ContinueMessageButton.cpp
        HUD/Buttons/MessagePage/ContinueMessageButton.h
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
        Component/Disabler.cpp Managers/LightsManager.h Managers/LightsManager.cpp Component/GlowstickMovement.h Component/GlowstickMovement.cpp
        Component/PDAController.cpp
        Component/PDAController.h
        HUD/Pages/PDA/PDAPage.cpp
        HUD/Pages/PDA/PDAPage.h)

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
