project(oneclickui)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(oneclickui_SOURCES main.cpp mainwindow.cpp firstscreen.cpp)
add_executable(oneclickui ${QT_LIBRARIES} ${oneclickui_SOURCES})

target_link_libraries(oneclickui ${QT_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
