cmake_minimum_required(VERSION 3.30)

include(_.cmake)
ae2f_CoreLibFetch(ae2f Core main)
ae2f_CoreLibFetch(ae2f Macro main)
ae2f_CoreLibFetch(ae2f Inet main)
ae2f_CoreLibFetch(ae2f Ruler main)

project(Bare)

configure_file(${PROJECT_SOURCE_DIR}/auto/Max.h ${PROJECT_SOURCE_DIR}/inc/Max.auto.h)
configure_file(${PROJECT_SOURCE_DIR}/auto/Dbg.h ${PROJECT_SOURCE_DIR}/inc/Dbg.auto.h)
configure_file(${PROJECT_SOURCE_DIR}/auto/Player.h ${PROJECT_SOURCE_DIR}/inc/Player.auto.h)
configure_file(${PROJECT_SOURCE_DIR}/auto/Room.h ${PROJECT_SOURCE_DIR}/inc/Room.auto.h)
configure_file(${PROJECT_SOURCE_DIR}/auto/test/port.h ${PROJECT_SOURCE_DIR}/test/port.auto.h)

file(GLOB incCore-src ${PROJECT_SOURCE_DIR}/inc/*)
file(GLOB incSvr-src ${PROJECT_SOURCE_DIR}/inc/Svr/*)
file(GLOB incCli-src ${PROJECT_SOURCE_DIR}/inc/Cli/*)

file(GLOB libCore-src ${PROJECT_SOURCE_DIR}/src/Core/*)
file(GLOB libSvrUDP-src ${PROJECT_SOURCE_DIR}/src/Svr/*)
file(GLOB libCli-src ${PROJECT_SOURCE_DIR}/src/Cli/*)

add_library(incCore INTERFACE ${incCore-src})
target_include_directories(incCore  INTERFACE ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(incCore       INTERFACE Inet)
target_link_libraries(incCore       INTERFACE Macro)

add_library(incSvr  INTERFACE ${incSvr-src})
add_library(incCli  INTERFACE ${incCli-src})

target_link_libraries(incSvr INTERFACE incCore)
target_link_libraries(incCli INTERFACE incCore)

add_library(libSvrUDP ${ae2f_LIBPREFIX} ${libSvrUDP-src})
add_library(libHost   ${ae2f_LIBPREFIX} ${libCli-src})
add_library(libCli    ${ae2f_LIBPREFIX} ${libCli-src})
add_library(libCore    ${ae2f_LIBPREFIX} ${libCore-src})

target_compile_definitions(libSvrUDP PRIVATE SERVER=1)
target_compile_definitions(libHost PRIVATE SERVER=1)
target_compile_definitions(libCli PRIVATE SERVER=0)

target_link_libraries(libCore PUBLIC incCore)
target_link_libraries(libCli PUBLIC incCli PUBLIC libCore)
target_link_libraries(libSvrUDP PUBLIC incSvr PUBLIC libCore)
target_link_libraries(libHost PUBLIC incCli PUBLIC libSvrUDP)

if(WIN32)
    target_link_libraries(libCli PUBLIC ws2_32)
    target_link_libraries(libCli PUBLIC synchronization)
    target_link_libraries(libSvrUDP PUBLIC ws2_32)
    target_link_libraries(libSvrUDP PUBLIC synchronization)
endif()

add_executable(testLobby test/main.c)
target_link_libraries(testLobby PUBLIC libSvrUDP)

if(NOT WIN32)
    add_executable(testWaitLinux test/TestWaitLinux.c)
    target_link_libraries(testWaitLinux PUBLIC incCore)
endif()

ae2f_CoreTestTent(libCli ${PROJECT_SOURCE_DIR}/test/units)


ae2f_Macro_init(1 100 100)
ae2f_Macro_one(${PROJECT_SOURCE_DIR}/inc/Room.imp.h ${PROJECT_SOURCE_DIR}/inc/Room.imp.auto.h)
