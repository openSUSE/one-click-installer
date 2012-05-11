project(backend)
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(backend_SOURCES package.cpp repository.cpp YmpParser.cpp)
#add_executable(backend ${QT_LIBRARIES} ${backend_SOURCES})

target_link_libraries(backend ${QT_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
