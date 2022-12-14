cmake_minimum_required(VERSION 3.5)

project(EdiQ VERSION 0.1 LANGUAGES CXX)

if(CMAKE_COMPILER_IS_GNUCXX)
    SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror=return-type")
    add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# SingleApplication base class
set(QAPPLICATION_CLASS QApplication)
add_subdirectory(SingleApplication)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h
        CodeEditor.cpp CodeEditor.h
        IEditorFactory.h IEditor.h
        EditorFactory.cpp EditorFactory.h
        UntitledCounter.cpp UntitledCounter.h
        EdiException.h
        CodeEditorSidebar.cpp CodeEditorSidebar.h
        PlainTextEdit.h
        SearchBar.cpp SearchBar.h
        Popup.cpp Popup.h
        raise.cpp raise.h XRaise.cpp XRaise.h
        een.cpp een.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
            res/icons.qrc
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
                res/icons.qrc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
        /usr/include/KF5/KSyntaxHighlighting
        /usr/include/botan-2)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        SingleApplication::SingleApplication
        KF5SyntaxHighlighting
        X11 botan-2
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
