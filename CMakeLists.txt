cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(simple_pool DESCRIPTION "Simple Pool game made with C++/SDL2" LANGUAGES CXX VERSION 0.1.0)

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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(SOURCES
    src/exception.cpp
    src/mainwindow.cpp
    src/texture.cpp
    src/font.cpp
    src/music.cpp
    src/gamestate.cpp
    src/intro.cpp
    src/level.cpp
    src/table.cpp
    src/subject.cpp
    src/observer.cpp
    src/rect.cpp
    src/ball.cpp
    src/collisionobserver.cpp
    src/audioobserver.cpp
    src/cue.cpp

    src/quartic.cpp
    src/collision.cpp
    src/matrix.cpp
    src/searchTable.cpp
    src/pocket.cpp
)

set(TEST_SOURCES
    test/collision/quadratic_next.cpp
    test/collision/next_collision_balls.cpp
    test/collision/next_collision_rect.cpp
)

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        $<$<CONFIG:Debug>:
            -O0 -g>
        $<$<CONFIG:Release>:
            -O3 -pipe -march=native>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

target_link_libraries(${PROJECT_NAME}
                      ${SDL2_LIBRARY}
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES}
                      ${SDL2_MIXER_LIBRARIES}
)

add_executable(test ${SOURCES} ${TEST_SOURCES})

target_link_libraries(  test
                        gtest_main
                        ${SDL2_LIBRARY}
                        ${SDL2_IMAGE_LIBRARIES}
                        ${SDL2_TTF_LIBRARIES}
                        ${SDL2_MIXER_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/pool_assets/
    DESTINATION ${CMAKE_INSTALL_BINDIR}/pool_assets/
)
