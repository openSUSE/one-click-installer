project( oneclickinstaller )
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(oneclickui_SOURCES main.cpp mainwindow.cpp firstscreen.cpp settings.cpp fakebackend.cpp repository.cpp package.cpp ympparser.cpp packagebackend.cpp installscreen.cpp details.cpp summary.cpp backend.cpp keyringcallbacks.cpp)
set(oneclickui_HEADERS mainwindow.h firstscreen.h settings.h fakebackend.h packagebackend.h package.h repository.h ympparser.h installscreen.h details.h summary.h backend.h keyringcallbacks.h keyring.h)
QT4_WRAP_CPP(oneclickui_HEADERS_MOC ${oneclickui_HEADERS})

add_executable(oneclickui ${QT_LIBRARIES} ${oneclickui_SOURCES} ${oneclickui_HEADERS} ${oneclickui_HEADERS_MOC})

target_link_libraries(oneclickui ${QT_LIBRARIES} zypp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
