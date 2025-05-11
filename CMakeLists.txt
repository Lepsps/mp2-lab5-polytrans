cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME polynoms)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Debug)

file(GLOB polynoms_hdrs "include/polynoms/*.h")
file(GLOB polynoms_srcs "include/polynoms/*.cpp")
file(GLOB cyclic_list_hdrs "include/cyclic_list/*.h")
file(GLOB cyclic_list_srcs "include/cyclic_list/*.cpp")
file(GLOB map_hdrs "include/map/*.h")
file(GLOB map_srcs "include/map/*.cpp")
file(GLOB tree_hdrs "include/tree/*.h")
file(GLOB tree_srcs "include/tree/*.cpp")
file(GLOB main_srcs "src/main.cpp")

add_library(cyclic_list ${cyclic_list_hdrs} ${cyclic_list_srcs})
target_include_directories(cyclic_list PUBLIC include/cyclic_list)

add_library(map ${map_hdrs} ${map_srcs})
target_include_directories(map PUBLIC include/map)

add_library(tree ${tree_hdrs} ${tree_srcs})
target_include_directories(tree PUBLIC include/tree)

add_library(polynoms ${polynoms_srcs} ${polynoms_hdrs})
target_include_directories(polynoms PUBLIC include/polynoms)
target_link_libraries(polynoms PUBLIC cyclic_list)

add_executable(main ${main_srcs})
target_link_libraries(main PRIVATE cyclic_list polynoms map)

include(CTest)
enable_testing()

if (BUILD_TESTING)
	message(STATUS "Building tests")

	add_library(gtest STATIC gtest/gtest-all.cc)
	target_include_directories(gtest PUBLIC gtest)
	
	if((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
    		${CMAKE_CXX_COMPILER_ID} MATCHES "Clang") AND
    		(${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
    		set(pthread "-pthread")
			target_link_libraries(gtest INTERFACE ${pthread})
	endif()

	file(GLOB main_tests_file "test/*.cpp")
	file(GLOB_RECURSE cyclic_list_tests_files "test/cyclic_list/*.cpp")
	file(GLOB_RECURSE polynoms_tests_files "test/polynoms/*.cpp")
	file(GLOB_RECURSE map_tests_files "test/map/*.cpp")
	file(GLOB_RECURSE tree_tests_files "test/tree/*.cpp")
	source_group("Source Files/cyclic_list" FILES ${cyclic_list_tests_files})

	source_group("Source Files/polynoms" FILES ${polynoms_tests_files})
	
	source_group("Source Files/map" FILES ${map_tests_files})

	source_group("Source Files/tree" FILES ${tree_tests_files})
	
	add_executable(polynoms_test ${main_tests_file} ${cyclic_list_tests_files} ${polynoms_tests_files} ${map_tests_files} ${tree_tests_files})

	target_link_libraries(polynoms_test PRIVATE gtest cyclic_list polynoms map tree) 

else()
	message(STATUS "Skipping tests")
endif()

# REPORT
message( STATUS "")
message( STATUS "Configuration for ${PROJECT_NAME}")
message( STATUS "====================================")
message( STATUS "  Build type:      ${CMAKE_BUILD_TYPE}")
message( STATUS "  Build tests:     ${BUILD_TESTING}")
message( STATUS "  C++ Standard:    ${CMAKE_CXX_STANDARD}")
message( STATUS "")