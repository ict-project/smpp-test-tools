################################################################
cmake_minimum_required(VERSION 2.6)
################################################################
project(smpp-test-tools)

set(CMAKE_CXX_STANDARD 11)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_SOURCE_FILES_SMPP 
 ../libsmpp/source/AntHill.cpp                ../libsmpp/source/SMPPBindTransceiverResp.cpp    ../libsmpp/source/SMPPDeliverSMResp.cpp         ../libsmpp/source/SMPPResp.cpp
 ../libsmpp/source/AntHillSMPP.cpp            ../libsmpp/source/SMPPBindTransmitter.cpp        ../libsmpp/source/SMPPEncoder.cpp               ../libsmpp/source/SMPPStructures.cpp
 ../libsmpp/source/BinaryDecoder.cpp          ../libsmpp/source/SMPPBindTransmitterResp.cpp    ../libsmpp/source/SMPPEnquireLink.cpp           ../libsmpp/source/SMPPSubmitMulti.cpp
 ../libsmpp/source/BinaryEncoder.cpp          ../libsmpp/source/SMPPBroadcastSM.cpp            ../libsmpp/source/SMPPEnquireLinkResp.cpp       ../libsmpp/source/SMPPSubmitMultiResp.cpp
 ../libsmpp/source/BinaryOutputter.cpp        ../libsmpp/source/SMPPBroadcastSMResp.cpp        ../libsmpp/source/SMPPGenericNack.cpp           ../libsmpp/source/SMPPSubmitSM.cpp
 ../libsmpp/source/ByteBuffer.cpp             ../libsmpp/source/SMPPCancelBroadcastSM.cpp      ../libsmpp/source/SMPPHexOutputter.cpp          ../libsmpp/source/SMPPSubmitSMResp.cpp
 ../libsmpp/source/DecoderBase.cpp            ../libsmpp/source/SMPPCancelBroadcastSMResp.cpp  ../libsmpp/source/SMPPOutbind.cpp               ../libsmpp/source/SMPPTxtOutputter.cpp
 ../libsmpp/source/EncoderBase.cpp            ../libsmpp/source/SMPPCancelSM.cpp               ../libsmpp/source/SMPPOutputterBase.cpp         ../libsmpp/source/SMPPUnbind.cpp
 ../libsmpp/source/OutputterBase.cpp          ../libsmpp/source/SMPPCancelSMResp.cpp           ../libsmpp/source/SMPPPDU.cpp                   ../libsmpp/source/SMPPUnbindResp.cpp
 ../libsmpp/source/SMPPAlertNotification.cpp  ../libsmpp/source/SMPPCSVOutputter.cpp           ../libsmpp/source/SMPPQueryBroadcastSM.cpp      ../libsmpp/source/SMPPValidator.cpp
 ../libsmpp/source/SMPPBind.cpp               ../libsmpp/source/SMPPDataSM.cpp                 ../libsmpp/source/SMPPQueryBroadcastSMResp.cpp  ../libsmpp/source/SMPPXMLOutputter.cpp
 ../libsmpp/source/SMPPBindReceiver.cpp       ../libsmpp/source/SMPPDataSMResp.cpp             ../libsmpp/source/SMPPQuerySM.cpp               ../libsmpp/source/TextOutputter.cpp
 ../libsmpp/source/SMPPBindReceiverResp.cpp   ../libsmpp/source/SMPPDatatypes.cpp              ../libsmpp/source/SMPPQuerySMResp.cpp           ../libsmpp/source/ValidatorBase.cpp
 ../libsmpp/source/SMPPBindResp.cpp           ../libsmpp/source/SMPPDecoder.cpp                ../libsmpp/source/SMPPReplaceSM.cpp             ../libsmpp/source/XMLOutputter.cpp
 ../libsmpp/source/SMPPBindTransceiver.cpp    ../libsmpp/source/SMPPDeliverSM.cpp              ../libsmpp/source/SMPPReplaceSMResp.cpp
)

set(CMAKE_SOURCE_FILES_ICT
 ../libict/source/base64.cpp  ../libict/source/logger.cpp  ../libict/source/options.cpp  ../libict/source/time.cpp  ../libict/source/utf8.cpp ../libict/source/os.cpp
)

set(CMAKE_SOURCE_FILES_TOOL
 connection.cpp smpp-connection.cpp resolver.cpp client.cpp server.cpp main-function.cpp
)

add_library(smpp STATIC ${CMAKE_SOURCE_FILES_SMPP})
add_library(ict  STATIC ${CMAKE_SOURCE_FILES_ICT})

add_executable(smpp-test-server ${CMAKE_SOURCE_FILES_TOOL} smpp-app.cpp daemon-app.cpp smpp-test-server.cpp)
target_link_libraries(smpp-test-server pthread smpp ict ${Boost_LIBRARIES})

add_executable(smpp-test-client ${CMAKE_SOURCE_FILES_TOOL} smpp-app.cpp daemon-app.cpp smpp-test-client.cpp)
target_link_libraries(smpp-test-client pthread smpp ict ${Boost_LIBRARIES})

add_executable(smpp-test-cli ${CMAKE_SOURCE_FILES_TOOL} user-app.cpp smpp-test-cli.cpp)
target_link_libraries(smpp-test-cli pthread smpp ict ${Boost_LIBRARIES})

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