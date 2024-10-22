cmake_minimum_required(VERSION 3.5)

project(EdiQ VERSION 0.1 LANGUAGES CXX)

if(CMAKE_COMPILER_IS_GNUCXX)
    SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror=return-type")
    add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SingleApplication base class
set(QAPPLICATION_CLASS QApplication)

add_subdirectory(3rdParty/syntax-hi-lib)
add_subdirectory(3rdParty/SingleApplication)

find_package(QT NAMES Qt6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(BOTAN_DIR "/usr/include/botan-2")
IF(NOT EXISTS ${BOTAN_DIR})
    message(FATAL_ERROR "Not found ${BOTAN_DIR}")
ELSE()
    message(STATUS "Found ${BOTAN_DIR}")
ENDIF()

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
        een.cpp een.h
        PlainTextEdit.cpp
        MRU.cpp
        MRU.h)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
        res/icons.qrc
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        3rdParty/syntax-hi-lib/lib
        ${BOTAN_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets
        SingleApplication::SingleApplication
        SyntaxHighlighting
        X11 botan-2
        )
