# Main wxMathPlot CMakeLists.txt
# Manage project
#
# Author: Davide Rondini
# Last Update: 2009-01-15
# License: wxWindows license

# Set CMake flags to enable compatibility both with 2.4 and 2.6
cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

project(wxMathPlot)

# Create options to be chosen by the user:
if(UNIX)
    SET(LINUX_64_32_CROSSCOMPILE OFF CACHE BOOL "Cross compile from Linux x86_64 to Linux x86?")
    SET(GDB_DEBUG OFF CACHE BOOL "Build with gdb debugger support?")
endif(UNIX)
# Option for any platform
SET(MATHPLOT_SHARED OFF CACHE BOOL "Create wxMathPlot as a shared library?")
SET(MATHPLOT_DO_LOGGING OFF CACHE BOOL "Build with verbose debugging messages?")

if(LINUX_64_32_CROSSCOMPILE)
    message(STATUS "Cross compiling from Linux x86_64 to Linux x86")
     set(CMAKE_LIBRARY_PATH "/usr/lib")
     set(CMAKE_SYSTEM_LIBRARY_PATH "/usr/lib")
     set(CMAKE_SYSTEM_PREFIX_PATH "/usr")
else(LINUX_64_32_CROSSCOMPILE)
    message(STATUS "Native build")
endif(LINUX_64_32_CROSSCOMPILE)
# message(STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH})

SET(wxWidgets_USE_LIBS base core)
find_package(wxWidgets)
if(wxWidgets_FOUND)
#     message(STATUS ${wxWidgets_LIBRARIES})
    include(${wxWidgets_USE_FILE})
	if(MATHPLOT_SHARED)
		add_library(mathplot SHARED mathplot.cpp mathplot.h)
	else(MATHPLOT_SHARED)
		add_library(mathplot STATIC mathplot.cpp mathplot.h)
	endif(MATHPLOT_SHARED)
    if(LINUX_64_32_CROSSCOMPILE)
        set_source_files_properties(mathplot.cpp PROPERTIES COMPILE_FLAGS "-m32 -DwxSIZE_T_IS_UINT" )
        set_target_properties(mathplot PROPERTIES LINK_FLAGS "-m32 ") # -L${CMAKE_LIBRARY_PATH}
    endif(LINUX_64_32_CROSSCOMPILE)
    if(GDB_DEBUG)
        set_source_files_properties(mathplot.cpp PROPERTIES COMPILE_FLAGS "-g -ggdb -Wall -pg -O0" )
        set_target_properties(mathplot PROPERTIES LINK_FLAGS "-g -ggdb -Wall -pg -O0")
		if (MATHPLOT_DO_LOGGING)
			set_source_files_properties(mathplot.cpp PROPERTIES COMPILE_FLAGS "-g -ggdb -Wall  -pg -O0 -DMATHPLOT_DO_LOGGING" )
		endif(MATHPLOT_DO_LOGGING)
    endif(GDB_DEBUG)
    if(LINUX_64_32_CROSSCOMPILE AND GDB_DEBUG)
        set_source_files_properties(mathplot.cpp PROPERTIES COMPILE_FLAGS "-m32 -g -ggdb -pg -O0 -DwxSIZE_T_IS_UINT" )
        set_target_properties(mathplot PROPERTIES LINK_FLAGS "-m32  -g -ggdb -pg -O0")
		if (MATHPLOT_DO_LOGGING)
			set_source_files_properties(mathplot.cpp PROPERTIES COMPILE_FLAGS "-m32 -g -ggdb -Wall -pg -O0 -DMATHPLOT_DO_LOGGING" )
		endif(MATHPLOT_DO_LOGGING)
    endif(LINUX_64_32_CROSSCOMPILE AND GDB_DEBUG)
    target_link_libraries(mathplot ${wxWidgets_LIBRARIES})

    # Compile samples?
    SET( WXMATHPLOT_BUILD_EXAMPLES ON CACHE BOOL "Build examples?")
    IF(WXMATHPLOT_BUILD_EXAMPLES)
        add_subdirectory(samples)
    ENDIF(WXMATHPLOT_BUILD_EXAMPLES)
	
	# library installation
    if(UNIX)
        if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
            set(LIBRARY_APPEND_PATH lib64)
        else(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
            set(LIBRARY_APPEND_PATH lib)
        endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
		install(TARGETS mathplot
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${LIBRARY_APPEND_PATH}
            ARCHIVE DESTINATION ${LIBRARY_APPEND_PATH}
        )
        set(WXMATHPLOT_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/wxMathPlot/)
        install(FILES Doxyfile DESTINATION ${WXMATHPLOT_INSTALL_DIR}/)
		install(FILES mathplot.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)
		install(FILES samples/sample1/mp1.cpp samples/sample1/CMakeLists.txt DESTINATION ${WXMATHPLOT_INSTALL_DIR}/samples/sample1)
		install(FILES samples/sample2/mp2.cpp samples/sample2/CMakeLists.txt DESTINATION ${WXMATHPLOT_INSTALL_DIR}/samples/sample2)
		install(FILES samples/sample3/sample3.cpp samples/sample3/CMakeLists.txt samples/sample3/gridmap.png DESTINATION ${WXMATHPLOT_INSTALL_DIR}/samples/sample3)
    else(UNIX)
    endif(UNIX)

else(wxWidgets_FOUND)
    MESSAGE("wxWidgets not found!")
endif(wxWidgets_FOUND)
