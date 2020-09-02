add_subdirectory(drivers)

find_path(LIBUSB_INCLUDE_DIR
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")

find_library(LIBUSB_LIBRARY
        NAMES usb
        PATH_SUFFIXES "lib" "lib32" "lib64")

find_package(Threads REQUIRED)

find_package(Boost REQUIRED COMPONENTS system)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${LIBUSB_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${LIBUSB_LIBRARY})
link_libraries(${Boost_LIBRARIES})

add_executable (spectool_raw spectool_raw.c spectool_net_client.c)
target_link_libraries(spectool_raw ${LIBUSB_LIBRARY} Threads::Threads spectool_container)

add_executable (
spectool_ng 
spectool_ng.cpp 
spectool_wrapper.cpp 
spectool_wrapper.h
mainwindow.cpp
mainwindow.h
mainwindow.ui
        widgets/topowidget.h
        widgets/topowidget.cpp
        widgets/spectowidget.h
        widgets/spectowidget.cpp
)
target_link_libraries(
        spectool_ng
        PRIVATE
        ${LIBUSB_LIBRARY}
        Threads::Threads
        Boost::boost Boost::dynamic_linking
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        ${OPENGL_LIBRARIES}
)
