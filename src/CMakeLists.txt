set(Themisto_srcs
    Managers/ConfigManager.cpp
    Managers/LogManager.cpp
    Managers/ApplicationManager.cpp
    Managers/ResourceManager.cpp
    Managers/StateManager.cpp
    Managers/GuiManager.cpp
    Core/Utils.cpp
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
    fontconfig
)
