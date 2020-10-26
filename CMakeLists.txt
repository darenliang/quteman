cmake_minimum_required(VERSION 3.5)

project(quteman LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

include(conan/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.ui")

set(SOURCES ${SOURCES}
        "lib/QJsonModel/qjsonmodel.cpp"
        "lib/QJsonModel/qjsonmodel.h")

if (ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
else ()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets CONAN_PKG::cpr)
