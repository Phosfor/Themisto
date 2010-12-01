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
    Core/ObjectsManager.cpp
    

    # Environ
    World/WorldManager.cpp
    World/Environ/Object.cpp
    World/Environ/Rain.cpp
    World/Environ/Moon.cpp
    World/Environ/Stars.cpp
    World/Environ/Sky.cpp
    World/Environ/Leaves.cpp
    World/Environ/Clouds.cpp
    World/Environ/Birds.cpp
    World/Environ/Lightnings.cpp

    # Objects
    World/Objects/Grass.cpp
    World/Objects/Body.cpp

    # States
    World/States/MenuState.cpp
    World/States/PhysicState.cpp

    # Physic   
    Physic/BodyVisual.cpp
    Physic/BodyMaterial.cpp
    Physic/Impact.cpp
    Physic/BodyState.cpp
    Physic/AreaManager.cpp
    Physic/PhysicRain.cpp
    Physic/PhysicWind.cpp
    Physic/BodyPart.cpp
    
    # Debug
    Debug/DebugIO.cpp
    Debug/DebugDraw.cpp
    Debug/DebugInstruments.cpp
    Debug/EvaluteFunctions.cpp
    Debug/DebugWatcher.cpp
    Debug/Watch.cpp   
    
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

