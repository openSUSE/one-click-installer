CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_TESTING(TRUE)
find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

set(ParseTest_SRCS parsetest.cpp YmpParser.cpp package.cpp repository.cpp)
set(ParseTest_HEADERS parsetest.h)
QT4_WRAP_CPP(ParseTest_HEADERS_MOC ${ParseTest_HEADERS})
ADD_EXECUTABLE(ParseTest ${ParseTest_SRCS} ${ParseTest_HEADERS_MOC})
TARGET_LINK_LIBRARIES(ParseTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} ${PARSETEST_SRCS})

ADD_TEST(NAME ParseTest COMMAND ParseTest)
