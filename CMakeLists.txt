cmake_minimum_required(VERSION 3.28)
project(Mygale)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

#add the source files
set(SOURCES
        src/main.cpp
        src/App/AppController.cpp
        src/App/MainWindow.cpp
        src/JobCreation/JobCreationWidget.cpp
        src/JobCreation/JobCreation.cpp
        src/JobCreation/Job.cpp
        src/Script/BaseScript.cpp
        src/AppSettings/AppSettings.cpp
        src/AppSettings/AppSettingsWidget.cpp
)

#add the header files
set(HEADERS
        src/App/AppController.h++
        src/App/MainWindow.h++
        src/JobCreation/JobCreationWidget.h++
        src/JobCreation/JobCreation.h++
        src/JobCreation/Job.h++
        src/Script/BaseScript.h++
        src/exception/CustomErrors.h++
        src/AppSettings/AppSettings.h++
        src/AppSettings/AppSettingsWidget.h++
)

#add the ui files
set(UIS
        resources/ui/MainWindow.ui
        resources/ui/JobCreationWidget.ui
        resources/ui/AppSettingsWidget.ui
)

#add the qrc files
set(QRC
        resources/resources.qrc
)

qt_add_executable(Mygale
        ${SOURCES}
        ${HEADERS}
        ${UIS}
        ${QRC}
)

target_link_libraries(Mygale
       PRIVATE Qt::Core
       PRIVATE Qt::Gui
       PRIVATE Qt::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
