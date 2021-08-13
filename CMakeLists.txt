cmake_minimum_required(VERSION 3.19)
project(live)

set(CMAKE_CXX_STANDARD 11)

include_directories(BasicUsageEnvironment/include)
include_directories(groupsock/include)
include_directories(liveMedia)
include_directories(liveMedia/include)
include_directories(mediaServer)
include_directories(testProgs)
include_directories(UsageEnvironment/include)

add_executable(live555MediaServer
        mediaServer/live555MediaServer.cpp
        mediaServer/DynamicRTSPServer.cpp
        mediaServer/testRTSPClient.cpp
        )
find_library(GROUP_SOCK_LIB NAMES groupsock HINTS groupsock)
find_library(LIVE_MEDIA NAMES liveMedia  HINTS liveMedia)
find_library(BAC_USG_ENV NAMES BasicUsageEnvironment HINTS BasicUsageEnvironment )
find_library(USG_ENV NAMES UsageEnvironment HINTS UsageEnvironment )
target_link_libraries(
        live555MediaServer
#        ${LIVE_MEDIA}
#        ${USG_ENV}
#        ${GROUP_SOCK}
#        ${BAC_USG_ENV}
        /home/ubuntu/Work/live/liveMedia/libliveMedia.a
        /home/ubuntu/Work/live/groupsock/libgroupsock.a
        /home/ubuntu/Work/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
        /home/ubuntu/Work/live/UsageEnvironment/libUsageEnvironment.a
        ssl
        crypto)