CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_TESTING(TRUE)
find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

set(InstallTest_SRCS installtest.cpp ../../src/backend/package.cpp)
set(InstallTest_HEADERS installtest.h)
QT4_WRAP_CPP(InstallTest_HEADERS_MOC ${InstallTest_HEADERS})
ADD_EXECUTABLE(InstallTest ${InstallTest_SRCS} ${InstallTest_HEADERS_MOC})
TARGET_LINK_LIBRARIES(InstallTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} zypp)

ADD_TEST(NAME InstallTest COMMAND InstallTest)
