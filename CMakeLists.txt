cmake_minimum_required(VERSION 3.16)
project(TimeTrackingApp LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    #UI
    ui/TimeTrackingApp.ui
    ui/ProjectWidget.ui
    #HEADERS
    headers/TimeTrackingApp.h
    headers/ProjectWidget.h
    headers/ProjectData.h
    headers/Session.h
    headers/utils.h
    #SRC
    src/main.cpp
    src/TimeTrackingApp.cpp
    src/ProjectWidget.cpp
    src/ProjectData.cpp
    src/Session.cpp
    #RESOURCES
    resources/Resources.qrc
)

qt_add_executable(TimeTrackingApp ${PROJECT_SOURCES})

set_target_properties(TimeTrackingApp
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(TimeTrackingApp
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
