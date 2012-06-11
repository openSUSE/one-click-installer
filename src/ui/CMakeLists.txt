project(oneclickui)
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(oneclickui_SOURCES main.cpp mainwindow.cpp firstscreen.cpp settings.cpp ../../src/backend/FakeBackend.cpp)
set(oneclickui_HEADERS mainwindow.h firstscreen.h settings.h ../../src/backend/FakeBackend.h ../../src/backend/AbstractBackend.h)
QT4_WRAP_CPP(oneclickui_HEADERS_MOC ${oneclickui_HEADERS})

add_executable(oneclickui ${QT_LIBRARIES} ${oneclickui_SOURCES} ${oneclickui_HEADERS} ${oneclickui_HEADERS_MOC})

target_link_libraries(oneclickui ${QT_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
