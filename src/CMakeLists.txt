rock_library(vizkit3d_laser_line
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(vizkit3d_laser_line_bin Main.cpp
    DEPS vizkit3d_laser_line)

