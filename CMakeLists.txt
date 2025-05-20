cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME TRASH)
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
file(GLOB hash_hdrs "include/hash/*.h")
file(GLOB hash_srcs "include/hash/*.cpp")
file(GLOB trans_hdrs "include/trans/*.h")
file(GLOB trans_srcs "include/trans/*.cpp")
file(GLOB main_srcs "src/main.cpp")

add_library(cyclic_list ${cyclic_list_hdrs} ${cyclic_list_srcs})
target_include_directories(cyclic_list PUBLIC include/cyclic_list)

add_library(map ${map_hdrs} ${map_srcs})
target_include_directories(map PUBLIC include/map)

add_library(tree ${tree_hdrs} ${tree_srcs})
target_include_directories(tree PUBLIC include/tree)

add_library(hash ${hash_hdrs} ${hash_srcs})
target_include_directories(hash PUBLIC include/hash)

add_library(polynoms ${polynoms_srcs} ${polynoms_hdrs})
target_include_directories(polynoms PUBLIC include/polynoms)
target_link_libraries(polynoms PUBLIC cyclic_list)

add_library(trans ${trans_hdrs} ${trans_srcs})
target_include_directories(trans PUBLIC include/trans)
target_link_libraries(trans PUBLIC polynoms)

add_executable(main ${main_srcs})
target_link_libraries(main PRIVATE cyclic_list polynoms map tree hash trans)

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
	file(GLOB_RECURSE hash_tests_files "test/hash/*.cpp")
	source_group("Source Files/cyclic_list" FILES ${cyclic_list_tests_files})

	source_group("Source Files/polynoms" FILES ${polynoms_tests_files})
	
	source_group("Source Files/map" FILES ${map_tests_files})

	source_group("Source Files/tree" FILES ${tree_tests_files})

	source_group("Source Files/hash" FILES ${hash_tests_files})
	
	add_executable(all_test ${main_tests_file} ${cyclic_list_tests_files} ${polynoms_tests_files} ${map_tests_files} ${tree_tests_files} ${hash_tests_files})

	target_link_libraries(all_test PRIVATE gtest cyclic_list polynoms map tree hash trans) 

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