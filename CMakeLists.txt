cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(simple_pool DESCRIPTION "Simple Pool game made with C++/SDL2" LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_BUILD_TYPE Debug)

set(GCC_COVERAGE_COMPILE_FLAGS "-flto -O2 -ffast-math -g")
set(GCC_COVERAGE_LINK_FLAGS    "-fuse-ld=lld -flto -O2 -ffast-math -Wl,-mllvm=-load=/Enzyme/enzyme/build/Enzyme/ClangEnzyme-12.so")
# set(GCC_COVERAGE_LINK_FLAGS    "-fuse-ld=lld -flto -ffast-math -Wl,-mllvm=-load=/Enzyme/enzyme/build/Enzyme/ClangEnzyme-12.so,-mllvm=-enzyme-print,-mllvm=-enzyme-preopt=0")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
                     ${SDL2_INCLUDE_DIR}
                     ${SDL2_IMAGE_INCLUDE_DIR}
                     ${SDL2_TTF_INCLUDE_DIR}
                     ${SDL2_MIXER_INCLUDE_DIRS}
)

set(SOURCES
    src/main.cpp
    src/exception.cpp
    src/mainwindow.cpp
    src/texture.cpp
    src/font.cpp
    src/music.cpp
    src/gamestate.cpp
    src/intro.cpp
    src/level.cpp
    src/levelNoGraphics.cpp
    src/forwardlevel.cpp
    src/table.cpp
    src/subject.cpp
    src/observer.cpp
    src/rect.cpp
    src/ball.cpp
    src/collisionobserver.cpp
    src/audioobserver.cpp
    src/cue.cpp

    src/gradient.cpp
    src/helpers.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
                      ${SDL2_LIBRARY}
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES}
                      ${SDL2_MIXER_LIBRARIES}
                      pthread
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/pool_assets/
    DESTINATION ${CMAKE_INSTALL_BINDIR}/pool_assets/
)
