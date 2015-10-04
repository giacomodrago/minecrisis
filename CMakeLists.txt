# Not tested on Windows
# Windows binaries available at https://github.com/giacomodrago/minecrisis/releases

# If you have problems with library dependencies, on Ubuntu you can try installing the following packages:
# * libgl-dev
# * freeglut3-dev
# * libpng-dev
# * libopenal-dev
# * libvorbis-dev

cmake_minimum_required(VERSION 2.8.7)
project(minecrisis)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake") 

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -pthread")
endif()

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

# libpng
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

# openal
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

# vorbis
find_package(OggVorbis REQUIRED)
include_directories(${OGGVORBIS_INCLUDE_DIR})

add_executable(minecrisis
    src/ase_importer.c
    src/explosion.c
    src/random.c
    src/utils.c
    src/bounding_boxes.c
    src/geom.c
    src/resources.c
    src/world.c
    src/crc32.c
    src/mine_crisis.c
    src/sound.c
    src/draw.c
    src/music.c
    src/texture.c
)

target_link_libraries(minecrisis
    ${OPENGL_LIBRARIES}
    ${GLUT_glut_LIBRARY}
    ${PNG_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${OGG_LIBRARY}
    ${VORBIS_LIBRARY}
    ${VORBISFILE_LIBRARY}
    m
)
