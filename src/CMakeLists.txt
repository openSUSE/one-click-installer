project( oneclickinstaller )
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(oneclickinstaller_SOURCES main.cpp mainwindow.cpp firstscreen.cpp settings.cpp fakebackend.cpp repository.cpp package.cpp ympparser.cpp packagebackend.cpp installscreen.cpp repositorywidget.cpp summary.cpp backend.cpp keyringcallbacks.cpp mainheader.cpp packagedetails.cpp)
set(oneclickinstaller_HEADERS mainwindow.h firstscreen.h settings.h fakebackend.h packagebackend.h package.h repository.h ympparser.h installscreen.h repositorywidget.h summary.h backend.h keyringcallbacks.h keyring.h mainheader.h packagedetails.h)

set(oneclickhelper_SOURCES interfacemain.cpp backend.cpp keyringcallbacks.cpp packagebackend.cpp)
set(oneclickhelper_HEADERS backend.h keyringcallbacks.h packagebackend.h)

QT4_WRAP_CPP(oneclickinstaller_HEADERS_MOC ${oneclickinstaller_HEADERS})
QT4_WRAP_CPP(onclickhelper_HEADERS_MOC ${backendinterface_HEADERS})

add_executable(oneclickhelper ${QT_LIBRARIES} ${oneclickhelper_SOURCES} ${oneclickhelper_HEADERS} ${oneclickhelper_HEADERS_MOC})
add_executable(oneclickinstaller ${QT_LIBRARIES} ${oneclickinstaller_SOURCES} ${oneclickinstaller_HEADERS} ${oneclickinstaller_HEADERS_MOC})

target_link_libraries(oneclickhelper ${QT_LIBRARIES} zypp)
target_link_libraries(oneclickinstaller ${QT_LIBRARIES} zypp)

INSTALL(TARGETS oneclickhelper DESTINATION /sbin)
INSTALL(TARGETS oneclickinstaller DESTINATION /bin)
#INSTALL_TARGETS(/sbin oneclickhelper)

if( UNIX AND NOT APPLE )
    if( NOT DESKTOP_ENTRY )
      set( DESKTOP_ENTRY oci.desktop )
    endif( NOT DESKTOP_ENTRY )

    add_custom_command( OUTPUT ${DESKTOP_ENTRY}
      COMMAND touch ${DESKTOP_ENTRY}
      COMMAND sh oci-desktop.sh ${CMAKE_INSTALL_PREFIX} >${DESKTOP_ENTRY}
      DEPENDS oci-desktop.sh
      COMMENT "Generating desktop entry file"
      )
    add_custom_target( DESKTOP_ENTRY_FILE ALL
      DEPENDS ${DESKTOP_ENTRY}
      )

    set( APP_ICON res/oneclickinstall.png )
  endif( UNIX AND NOT APPLE )

INSTALL(FILES ${DESKTOP_ENTRY} DESTINATION /usr/share/applications)
INSTALL(FILES res/oneclickinstall.png DESTINATION /usr/share/icons/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
