find_path(LIBUSB_INCLUDE_DIR
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")

find_library(LIBUSB_LIBRARY
        NAMES usb
        PATH_SUFFIXES "lib" "lib32" "lib64")


find_package(Threads REQUIRED)

include_directories(${LIBUSB_INCLUDE_DIR} ../)
link_directories(${LIBUSB_LIBRARY} ../)
link_libraries( -lm )

add_library (wispy_hw_24x wispy_hw_24x.c wispy_hw_24x.h)
add_library (wispy_hw_dbx wispy_hw_dbx.c wispy_hw_dbx.h)
add_library (wispy_hw_gen1 wispy_hw_gen1.c wispy_hw_gen1.h)
add_library (ubertooth_hw_u1 ubertooth_hw_u1.c ubertooth_hw_u1.h)
add_library (spectool_container spectool_container.c spectool_container.h)
target_link_libraries(spectool_container wispy_hw_24x wispy_hw_dbx wispy_hw_gen1 ubertooth_hw_u1)