cmake_minimum_required(VERSION 2.82)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SRC})

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vita-pong)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APPNAME  "Pong")
set(VITA_TITLEID  "HMRL00001")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

set(VITA_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/")
set(SDL2_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/SDL2")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${VITA_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  SDL2
  SceAudio_stub
  SceAudioIn_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceDisplay_stub
  SceGxm_stub
  SceHid_stub
  SceSysmodule_stub
  SceTouch_stub
  SceMotion_stub
)

vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk ${VITA_TITLEID} ${CMAKE_PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APPNAME}
  FILE assets/fonts assets/fonts
)
