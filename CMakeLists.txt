cmake_minimum_required(VERSION 3.0)
project(SHistogram)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT REQUIRED)

include(${ROOT_USE_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(SHistogram SHARED SHistoBuilder.cxx SHistoBuilder.h)
target_link_libraries(SHistogram PUBLIC ${ROOT_LIBRARIES})


install(
        TARGETS SHistogram
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
)

install(
        FILES SHistoBuilder.h
        DESTINATION includes
)


if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    enable_testing()

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_executable(tests test/TestRunner.cxx test/TestSHistoBuilder.h)
    target_link_libraries(tests SHistogram gtest)

    add_test(
            NAME TestSHistogram
            COMMAND tests
    )
endif () # CMAKE_BUILD_TYPE
