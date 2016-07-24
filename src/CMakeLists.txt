PROJECT(oneclickinstaller)

# Minimal requirements
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(REQUIRED_QT_VERSION 5.4.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

#Qt
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Widgets DBus)
set(QT_QTCORE_LIBRARY Qt5::Core)
set(QT_QTDBUS_LIBRARY Qt5::DBus)
set(QT_QTGUI_LIBRARY Qt5::Widgets)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5DBus_INCLUDES})
set(Qt5_USE_DBUS, true)

find_package(PACKAGEKITQT5 REQUIRED)
find_package(KF5I18n NO_MODULE)

#We need -DQT_WIDGETS_LIB when using QtWidgets in Qt5
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5DBus_DEFINITIONS})

set(oneclickinstaller_SOURCES main.cpp backendoci.cpp conflictresolutionscreen.cpp utils.cpp mainwindow.cpp firstscreen.cpp settings.cpp fakebackend.cpp repository.cpp package.cpp ympparser.cpp packagebackend.cpp installscreen.cpp repositorywidget.cpp summary.cpp keyringcallbacks.cpp mainheader.cpp packagedetails.cpp repositorymetadata.cpp repositorydata.cpp packagemetadata.cpp)
set(oneclickinstaller_HEADERS backendoci.h conflictresolutionscreen.h utils.h mainwindow.h checkconflictscreen.h firstscreen.h settings.h installscreen.h repositorywidget.h summary.h mainheader.h packagedetails.h packagemetadata.h packagebackend.h)

set(oneclickhelper_SOURCES interfacemain.cpp backend.cpp  utils.cpp)
set(oneclickhelper_HEADERS backend.h keyring.h utils.h)

QT5_WRAP_CPP(oneclickhelper_HEADERS_MOC ${oneclickhelper_HEADERS})
QT5_WRAP_CPP(oneclickinstaller_HEADERS_MOC ${oneclickinstaller_HEADERS})

# Dbus Adaptor(s) and interface(s)
set(srcs_DBUS_OCIHelper org.opensuse.OCIHelper.xml)
set(srcs_DBUS_OCI org.opensuse.oneclickinstaller.xml)
qt5_add_dbus_interface(prog_iFaceHelper org.opensuse.OCIHelper.xml oci_helper_interface)
qt5_add_dbus_interface(prog_iFaceOCI org.opensuse.oneclickinstaller.xml oci_interface)

qt5_add_dbus_adaptor(srcs_DBUS_OCIHelper org.opensuse.OCIHelper.xml backend.h Backend)
qt5_add_dbus_adaptor(srcs_DBUS_OCI org.opensuse.oneclickinstaller.xml conflictresolutionscreen.h ConflictResolutionScreen)

qt5_generate_moc(backend.h backend.moc)
qt5_generate_moc(conflictresolutionscreen.h conflictresolutionscreen.moc)

#Executables fail to build with Qt5 in the default configuration
#without -fPIE. We add that here
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(oneclickhelper ${QT_LIBRARIES} ${oneclickhelper_SOURCES} ${prog_iFaceOCI} ${oneclickhelper_HEADERS} ${srcs_DBUS_OCIHelper} ${oneclickhelper_HEADERS_MOC})
add_executable(oneclickinstaller ${QT_LIBRARIES} ${prog_iFaceHelper} ${srcs_DBUS_OCI} ${oneclickinstaller_SOURCES} ${oneclickinstaller_HEADERS} ${oneclickinstaller_HEADERS_MOC})

target_link_libraries(oneclickhelper ${QT_QTGUI_LIBRARY} zypp ${QT_QTDBUS_LIBRARY})
target_link_libraries(oneclickinstaller ${QT_QTGUI_LIBRARY} KF5::I18n zypp ${QT_QTDBUS_LIBRARY} ${PackageKitQt5_LIBRARIES})

INSTALL(TARGETS oneclickhelper DESTINATION /usr/sbin)
INSTALL(TARGETS oneclickinstaller DESTINATION /usr/bin)
#INSTALL_TARGETS(/sbin oneclickhelper)


INSTALL(FILES oci.desktop DESTINATION /usr/share/applications)
INSTALL(FILES res/oneclickinstall.png DESTINATION /usr/share/icons/hicolor/32x32/apps/)
INSTALL(FILES res/ticks-endless.gif DESTINATION /usr/share/one-click-installer/res/)
INSTALL(FILES res/conflictCheck.gif DESTINATION /usr/share/one-click-installer/res/)
INSTALL(FILES res/conflictIcon.png DESTINATION /usr/share/icons/hicolor/32x32/apps/)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})
