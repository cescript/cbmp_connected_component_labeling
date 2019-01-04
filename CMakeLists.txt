cmake_minimum_required(VERSION 3.6)
project(connected_component_labeling_cbmp_app CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set copiler flags for debug and release
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -pg -D_DEBUG -w")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O2 -w")

# set the output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bins")

set(CMAKE_BUILD_TYPE Release)

# set header and source files for the test
include_directories(${MAKE_CURRENT_LIST_DIR}/include/)

# get all .c files under each sub folder
file(GLOB test_files *.cpp)

#for each c file in the samples directory
foreach(test_target ${test_files})

    #get the test source file name from the path
    get_filename_component(test_source_name ${test_target} NAME)
    message(STATUS  ${test_source_name})

    # create an executable_name via omitting the posix
    string(REPLACE ".c" "" executable_name ${test_source_name})
    add_executable(${executable_name} ${test_source_name})

    # Make sure YourLib is linked to each app
    if(UNIX)
        target_link_libraries(${executable_name} m)
    endif(UNIX)
endforeach(test_target ${test_samples})
