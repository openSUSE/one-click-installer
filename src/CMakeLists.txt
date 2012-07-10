project( oneclickinstaller )
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(oneclickinstaller_SOURCES main.cpp mainwindow.cpp firstscreen.cpp settings.cpp fakebackend.cpp repository.cpp package.cpp ympparser.cpp packagebackend.cpp installscreen.cpp details.cpp summary.cpp backend.cpp keyringcallbacks.cpp)
set(oneclickinstaller_HEADERS mainwindow.h firstscreen.h settings.h fakebackend.h packagebackend.h package.h repository.h ympparser.h installscreen.h details.h summary.h backend.h keyringcallbacks.h keyring.h)

set(oneclickhelper_SOURCES interfacemain.cpp backend.cpp keyringcallbacks.cpp packagebackend.cpp)
set(oneclickhelper_HEADERS backend.h keyringcallbacks.h packagebackend.h)

QT4_WRAP_CPP(oneclickinstaller_HEADERS_MOC ${oneclickinstaller_HEADERS})
QT4_WRAP_CPP(onclickhelper_HEADERS_MOC ${backendinterface_HEADERS})

add_executable(oneclickhelper ${QT_LIBRARIES} ${oneclickhelper_SOURCES} ${oneclickhelper_HEADERS} ${oneclickhelper_HEADERS_MOC})
add_executable(oneclickinstaller ${QT_LIBRARIES} ${oneclickinstaller_SOURCES} ${oneclickinstaller_HEADERS} ${oneclickinstaller_HEADERS_MOC})

target_link_libraries(oneclickhelper ${QT_LIBRARIES} zypp)
target_link_libraries(oneclickinstaller ${QT_LIBRARIES} zypp)

INSTALL(FILES oneclickhelper DESTINATION /sbin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
