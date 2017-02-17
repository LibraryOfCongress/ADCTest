# wxMathPlot Sample2 CMakeLists.txt
#
# Author: Davide Rondini
# Last Update: 2008-02-20
# License: wxWindows license

# Set CMake flags to enable compatibility both with 2.4 and 2.6
cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

SET(wxWidgets_USE_LIBS base core)
find_package(wxWidgets)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})

	include_directories(../..)
	
	add_executable(mpSample2 mp2.cpp)
	# Under Windows, the target type must be WINDOWS, not CONSOLE
	if(WIN32 AND MSVC)
		set_target_properties(mpSample2 PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	endif(WIN32 AND MSVC)
	# Linux cross compiling to x86 from x86_64 distribution
	if(LINUX_64_32_CROSSCOMPILE)
		set_source_files_properties(mp2.cpp PROPERTIES COMPILE_FLAGS "-m32 -DwxSIZE_T_IS_UINT")
		set_target_properties(mpSample2 PROPERTIES LINK_FLAGS "-m32")
	endif(LINUX_64_32_CROSSCOMPILE)
	target_link_libraries(mpSample2 ${wxWidgets_LIBRARIES} mathplot)
endif(wxWidgets_FOUND)