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
    Core/InputManager.cpp
    Core/LevelManager.cpp
    Core/TemplatesProcessing.cpp
    Core/WorldManager.cpp
    Core/ScriptsManager.cpp

    # Objects
    World/Objects/TypesManager.cpp
    World/Objects/Camera.cpp
    #World/Objects/Grass.cpp
    #World/Objects/LightColumn.cpp
    #World/Objects/PhysicObject.cpp
    #World/Objects/Level.cpp
    #World/Objects/Player.cpp
    #World/Objects/Actor.cpp
    #World/Objects/Moon.cpp
    #World/Objects/Birds.cpp
    #World/Objects/Clouds.cpp
    #World/Objects/Rain.cpp
    #World/Objects/Foreground.cpp
    #World/Objects/Empty.cpp
    #World/Objects/Sky.cpp
    #World/Objects/Stars.cpp
    #World/Objects/Leaves.cpp

    # Actions
    #World/Actions/TakeAction.cpp
    World/Actions/TestAction.cpp

    # States
    World/States/MenuState.cpp
    #World/States/EditorState.cpp

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

    ${PYTHON_LIBRARIES}
    ${Boost_LIBRARIES}
    ${ClanLib_LIBRARIES}
    ${BOX2D_LIBRARY}
    fontconfig
    pthread
)

# ----------------------------------------------------
# Shared library for script system
# ----------------------------------------------------
set(Script_srcs
    Core/LogManager
    Core/ConfigManager
    Core/TypesConverters
    Core/Utils
    Core/ApplicationManager
    Core/GuiManager
    Core/InputManager
    Core/ResourceManager
    Core/WorldManager
    Core/LevelManager

    World/Objects/Object
    World/Objects/TypesManager

    Physic/Physic
)

# Additional deps for physic module
set(Physic_srcs
    Scripts/Physic/b2Body.cpp
    Scripts/Physic/b2Fixture.cpp
    Scripts/Physic/b2Joint.cpp
    Scripts/Physic/b2Shape.cpp
    Scripts/Physic/b2Vec2.cpp
    Scripts/Physic/b2Vertex.cpp
    Scripts/Physic/b2World.cpp

    Scripts/Physic/Physic.cpp
)

foreach(i ${Script_srcs})
    # Takes only filename from given filepath
    get_filename_component(fileName ${i} NAME_WE)
    # Takes only filepath (without filename)
    get_filename_component(filePath ${i} PATH)

    # Add library with dependencies
    if (${fileName} STREQUAL "Physic")
        add_library(${fileName} SHARED ${Physic_srcs})
    else()
        add_library(${fileName} SHARED Scripts/${i}.cpp)
    endif()

    # Put library into need folder hierachy
    set_target_properties(${fileName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib/${filePath})
    # Remove 'lib' preffix from the library name
    set_target_properties(${fileName} PROPERTIES PREFIX "")
    # Linker needs in own libraries
    target_link_libraries(${fileName}
        ${PYTHON_LIBRARIES}
        ${Boost_LIBRARIES}
    )
endforeach(i)

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
    ${PYTHON_LIBRARIES}
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
