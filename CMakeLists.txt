cmake_minimum_required(VERSION 2.8)
project(cppnetserver)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)

  if (HAVE_STD11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "No C++ 11 support (Compiler does not define -std=c++11).")
  endif()

  if (Uri_FULL_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif()

  if (Uri_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

  message("C++ Flags: ${CMAKE_CXX_FLAGS} link flags: ${CMAKE_CXX_LINK_FLAGS}")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  if (NOT Uri_DISABLE_LIBCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()

  if (Uri_FULL_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif()

  if (Uri_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

  message("C++ Flags: ${CMAKE_CXX_FLAGS} link flags: ${CMAKE_CXX_LINK_FLAGS}")
endif()


if(BUILD_FOR_ARM)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ljsoncpp")
endif()

# set test option
if(WIN32 OR APPLE)
  option(BUILD_CppTests "Only build TestCpp sample" OFF)
else()
  option(BUILD_CppTests "Only build TestCpp sample" OFF)
endif()

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# find OpenSSL, Threads, Boost, cppnetlib, jsoncpp
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTI_THREAD ON)

find_package( Boost 1.54.0
  REQUIRED unit_test_framework system regex date_time thread filesystem
  program_options chrono atomic log)

#find_package( OpenSSL )
#find_package( Threads )
#find_package( cppnetlib 0.11.0 REQUIRED )
#find_package( jsoncpp)



if (Boost_FOUND)
  # include some dirs
  if(BUILD_FOR_ARM)
    include_directories(
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      )
  else()
    include_directories(
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${Boost_INCLUDE_DIRS}
      ${CPPNETLIB_INLUDE_DIRS}
      )
      endif()
endif ()


if (OPENSSL_FOUND)
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif (OPENSSL_FOUND)


  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  include_directories("dep/cpp-netlib-0.12.0-final")
#  include_directories("dep/uri/include")


  # AVOSCloud library
#  add_subdirectory(src)
#link_directories("${PROJECT_BINARY_DIR}/dep/jsoncpp/src/lib_json")
link_directories("/Users/aadebuger/Documents/cpp-netlib-0.12.0-final/build/libs/network/src/")

add_subdirectory("dep/cpp-netlib-0.12.0-final")
#add_subdirectory("dep/uri")

add_executable( ${PROJECT_NAME} main.cpp )
#target_link_libraries(${PROJECT_NAME} jsoncpp)
target_link_libraries(${PROJECT_NAME} cppnetlib-uri)
target_link_libraries(${PROJECT_NAME} cppnetlib-server-parsers)
target_link_libraries(${PROJECT_NAME} cppnetlib-client-connections)


  # AVOSCloud headers
  install(DIRECTORY include DESTINATION ${CMAKE_BINARY_DIR})

  # cpp test
  if(BUILD_CppTests)
    enable_testing()
    add_subdirectory(tests)
  endif(BUILD_CppTests)
