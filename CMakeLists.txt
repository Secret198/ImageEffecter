cmake_minimum_required(VERSION 3.16)

project(ImageEffecter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick GuiPrivate ShaderTools)

qt_standard_project_setup(REQUIRES 6.10)

# Content of downloaded design from Figma to Qt or similar
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(appImageEffecter
    main.cpp
)

qt_add_qml_module(appImageEffecter
    URI ImageEffecter
    QML_FILES
        UI_components/Main.qml
        QML_FILES UI_components/AddEffectBox.qml
        QML_FILES UI_components/ImageContainer.qml
        QML_FILES UI_components/EffectInstance.qml

    SOURCES
        computeitem.h
        computeitem.cpp


    RESOURCES
        assets/icons/cross_icon.svg
        assets/icons/plus_icon.svg
        assets/test.jpg
        assets/test2.jpg

        SOURCES computerenderer.h computerenderer.cpp
        SOURCES computeitem.h computeitem.cpp
        SOURCES effect.h effect.cpp
        SOURCES effectcontainer.h effectcontainer.cpp

)

qt_add_shaders(appImageEffecter "shaders"
    PREFIX "/"
    GLSL "310es,430" HLSL 50 MSL 12
    FILES
        shaders/quad.vert
        shaders/quad.frag
        shaders/test_compute.comp
        shaders/test_compute2.comp
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appImageEffecter PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appImageEffecter
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appImageEffecter
    PRIVATE Qt6::Quick Qt6::GuiPrivate
)

install(TARGETS appImageEffecter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
