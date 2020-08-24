add_subdirectory(drivers)

find_path(LIBUSB_INCLUDE_DIR
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")

find_library(LIBUSB_LIBRARY
        NAMES usb
        PATH_SUFFIXES "lib" "lib32" "lib64")

find_package(Threads REQUIRED)

include_directories(${LIBUSB_INCLUDE_DIR})
link_directories(${LIBUSB_LIBRARY})

add_executable (spectool_raw spectool_raw.c spectool_net_client.c)
target_link_libraries(spectool_raw ${LIBUSB_LIBRARY} Threads::Threads spectool_container)

add_executable (spectool_ng spectool_ng.cpp)
target_link_libraries(spectool_ng ${LIBUSB_LIBRARY} Threads::Threads spectool_container)
