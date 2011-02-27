# ----------------------------------------------------
# Executable for the Themisto
# ----------------------------------------------------
set(Themisto_srcs
    # Core
    Core/ConfigManager.cpp
    Core/LogManager.cpp
    Core/ApplicationManager.cpp
    Core/ResourceManager.cpp
    Core/StateManager.cpp
    Core/GuiManager.cpp
    Core/Utils.cpp
    Core/PhysicManager.cpp
    Core/SceneLoader.cpp
    Core/EnvironManager.cpp
    Core/InputManager.cpp
    Core/LevelManager.cpp
    Core/TemplatesProcessing.cpp

    # Environ
    World/WorldManager.cpp
    World/Environ/Object.cpp
    #World/Environ/Rain.cpp
    #World/Environ/Moon.cpp
    World/Environ/Stars.cpp
    World/Environ/Sky.cpp
    World/Environ/Leaves.cpp
    World/Environ/Lightnings.cpp
    #World/Environ/Foreground.cpp
    World/Environ/Objects.cpp

    # Objects
    World/Objects/Grass.cpp
    World/Objects/LightColumn.cpp
    World/Objects/TypesManager.cpp
    World/Objects/PhysicObject.cpp
    World/Objects/Player.cpp
    World/Objects/Actor.cpp
    World/Objects/Moon.cpp
    World/Objects/Birds.cpp
    World/Objects/Clouds.cpp
    World/Objects/Rain.cpp
    World/Objects/Foreground.cpp
    World/Objects/Empty.cpp

    # Actions
    #World/Actions/TakeAction.cpp

    # States
    World/States/MenuState.cpp
    World/States/EditorState.cpp

    # Physic
    Physic/PhysicRain.cpp
    Physic/PhysicWind.cpp
    Physic/Body.cpp
    Physic/PhysicParser.cpp

    # Debug
    Debug/DebugDraw.cpp
    Debug/DebugInstruments.cpp

    main.cpp
)

add_executable(Themisto
    ${Themisto_srcs}
)

target_link_libraries(Themisto
    ${X11_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREETYPE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${PNG_LIBRARIES}
    ${JPEG_LIBRARIES}

    ${Boost_LIBRARIES}
    ${ClanLib_LIBRARIES}
    ${BOX2D_LIBRARY}
    fontconfig
    pthread
)

# ----------------------------------------------------
# Executable for the DebugConsole
# ----------------------------------------------------
set(DebugConsole_srcs
    Debug/DebugConsole.cpp
)

add_executable(DebugConsole
    ${DebugConsole_srcs}
)

target_link_libraries(DebugConsole
    ${X11_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREETYPE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${PNG_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${ClanLib_LIBRARIES}
    ${READLINE_LIBRARIES}
    ${Boost_LIBRARIES}
    readline
    history
    ncurses
    fontconfig
    pthread
)

# ----------------------------------------------------
# Executable for the DebugVisualisator
# ----------------------------------------------------
set(DebugVisualisator_srcs
    Debug/Visualisator/main.cpp
    Debug/Visualisator/Client.cpp

    Core/GuiManager.cpp
    Core/ConfigManager.cpp
)

add_executable(DebugVisualisator
    ${DebugVisualisator_srcs}
)

target_link_libraries(DebugVisualisator
    ${X11_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${FREEIMAGE_LIBRARY}
    ${FREETYPE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${PNG_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${ClanLib_LIBRARIES}
    ${READLINE_LIBRARIES}
    fontconfig
    pthread
)
