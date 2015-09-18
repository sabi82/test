





cmake_minimum_required (VERSION 2.6)

#Declaration du projet
project(sixth)

# The version number.
set (Sixth_VERSION_MAJOR 1)
set (Sixth_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file ("${PROJECT_SOURCE_DIR}/SixthConfig.h.in" "${PROJECT_BINARY_DIR}/SixthConfig.h")

# Déclaration de la lib
add_library(PngLib readwritepng.c)
set (EXTRA_LIBS ${EXTRA_LIBS} PngLib)


## Link libraries
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(PngLib ${PNG_LIBRARY})


#Déclaration de l'exécutable
add_executable(
	sixth
	main.c
)
target_link_libraries (sixth  ${EXTRA_LIBS})
