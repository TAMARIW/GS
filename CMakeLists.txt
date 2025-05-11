cmake_minimum_required(VERSION 3.19)
project(dock-gs LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets PrintSupport)

qt_standard_project_setup()

qt_add_executable(dock-gs
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
    qcustomplot.cpp qcustomplot.h
)

qt_add_resources(RESOURCE_FILES resources.qrc)

target_include_directories(dock-gs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(dock-gs PRIVATE
    ${RESOURCE_FILES}
)

target_link_libraries(dock-gs
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::PrintSupport
)

include(GNUInstallDirs)

install(TARGETS dock-gs
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET dock-gs
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
