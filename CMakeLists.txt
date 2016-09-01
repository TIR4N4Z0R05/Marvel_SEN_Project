cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
project( MARVEL_SEN_PROJECT )
find_package( OpenCV 2 REQUIRED )
find_package(PCL 1.7 REQUIRED)
find_package(MRPT REQUIRED)
find_package( Boost REQUIRED COMPONENTS program_options regex )
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${SDL2_INCLUDE_DIRS} )

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()
rosbuild_gensrv()
rosbuild_genmsg()
#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)


#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(out src/talker.cpp)
rosbuild_add_executable(PH src/PositionHold.cpp)
rosbuild_add_executable(permanent src/permanent.cpp)
rosbuild_add_executable(Converter src/ConvertToEuler.cpp)
rosbuild_add_executable(2D_map src/ReadMap.cpp)
rosbuild_add_executable(CM src/costmap.cpp)
rosbuild_add_executable(ANT src/ANT.cpp)
rosbuild_add_executable(interface src/interface.cpp)
#target_link_libraries(example ${PROJECT_NAME})
#target_link_libraries( out ${OpenCV_LIBS} )
target_link_libraries(2D_map ${OpenCV_LIBS} ${PCL_LIBRARIES} ${MRPT_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(CM ${OpenCV_LIBS} ${PCL_LIBRARIES} ${MRPT_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(ANT ${OpenCV_LIBS} ${PCL_LIBRARIES} ${MRPT_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY} ${SDL2_LIBRARIES})
target_link_libraries(interface ${OpenCV_LIBS} ${PCL_LIBRARIES} ${MRPT_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY} ${SDL2_LIBRARIES})
target_link_libraries(2D_map GL glut)
target_link_libraries(CM GL glut)
target_link_libraries(ANT GL glut)
