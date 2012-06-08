CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_TESTING(TRUE)
find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

set(KeyRingTest_SRCS keyringtest.cpp  repository.cpp keyringcallbacks.cpp)
set(KeyRingTest_HEADERS keyringtest.h)
QT4_WRAP_CPP(KeyRingTest_HEADERS_MOC ${KeyRingTest_HEADERS})
ADD_EXECUTABLE(KeyRingTest ${KeyRingTest_SRCS} ${KeyRingTest_HEADERS_MOC})
TARGET_LINK_LIBRARIES(KeyRingTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} ${KeyRingTest_SRCS} zypp)

ADD_TEST(NAME KeyRingTest COMMAND KeyRingTest)
