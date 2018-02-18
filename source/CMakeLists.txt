################################################################
cmake_minimum_required(VERSION 2.6)
################################################################
project(smpp-test-tools)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ICT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libict/)
set(CMAKE_ICT_SOURCE ${CMAKE_ICT_DIR}/source/)
set(CMAKE_ICT_BIN ${CMAKE_ICT_DIR}/build/)
add_subdirectory(${CMAKE_ICT_SOURCE} ${CMAKE_ICT_BIN})

set(CMAKE_SMPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libsmpp/)
set(CMAKE_SMPP_SOURCE ${CMAKE_SMPP_DIR}/source/)
set(CMAKE_SMPP_BIN ${CMAKE_SMPP_DIR}/build/)
add_subdirectory(${CMAKE_SMPP_SOURCE} ${CMAKE_SMPP_BIN})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_SOURCE_FILES_TOOL
 connection.cpp smpp-connection.cpp resolver.cpp client.cpp server.cpp main-function.cpp
)

add_executable(smpp-test-server ${CMAKE_SOURCE_FILES_TOOL} smpp-app.cpp daemon-app.cpp smpp-test-server.cpp)
target_link_libraries(smpp-test-server pthread smpp-static ict-static ${Boost_LIBRARIES})

add_executable(smpp-test-client ${CMAKE_SOURCE_FILES_TOOL} smpp-app.cpp daemon-app.cpp smpp-test-client.cpp)
target_link_libraries(smpp-test-client pthread smpp-static ict-static ${Boost_LIBRARIES})

add_executable(smpp-test-cli ${CMAKE_SOURCE_FILES_TOOL} user-app.cpp smpp-test-cli.cpp)
target_link_libraries(smpp-test-cli pthread smpp-static ict-static ${Boost_LIBRARIES})

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} describe
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      OUTPUT_VARIABLE "GIT_VERSION"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message( STATUS "Git version: ${GIT_VERSION}" )
  else(GIT_FOUND)
    set(GIT_VERSION unknown)
  endif(GIT_FOUND)
else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)
 set(GIT_VERSION unknown)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/git_version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/git_version.h @ONLY)

install(TARGETS smpp-test-server smpp-test-client smpp-test-cli DESTINATION bin COMPONENT tools)
################################################################
#enable_testing()
#set(CMAKE_CURRENT_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tests)
#set(CMAKE_CURRENT_TEST_TOOL ${CMAKE_CURRENT_BINARY_DIR}/libict-test)

#add_test(NAME ict-utf8-tc1 WORKING_DIRECTORY ${CMAKE_CURRENT_TESTS_DIR}  COMMAND ${CMAKE_CURRENT_TEST_TOOL} ict utf8 tc1)
################################################################
#set(CPACK_GENERATOR "TZ;TGZ;STGZ;DEB;RPM") # Linux
#set(CPACK_GENERATOR "TZ;TGZ;STGZ")         # MacOS

set(CPACK_PACKAGE_NAME "smpp-test-tools")
set(CPACK_PACKAGE_VENDOR "ict-project.pl")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mariusz Ornowski <mariusz.ornowski@ict-project.pl>")
set(CPACK_RPM_PACKAGE_LICENSE "BSD 3-clause")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "smpp-test-tools - SMPP testing tool kit.")
set(CPACK_PACKAGE_VERSION ${GIT_VERSION})

set(CPACK_COMPONENTS_ALL tools)

set(CPACK_COMPONENT_TOOLS_DISPLAY_NAME "SMPP tools")
set(CPACK_COMPONENT_TOOLS_DESCRIPTION "SMPP tools - SMPP testing tool kit.")

include(CPack)
################################################################