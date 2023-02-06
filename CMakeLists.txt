cmake_minimum_required(VERSION 3.24.1)

#----------------------------------------------------
#			   	 PROJECT PROPERTIES
#----------------------------------------------------
project(acaer VERSION 0.1.1)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

## Requierments ## 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# Set release and debug folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# COMPILER FLAGS
#
set(CMAKE_CXX_FLAGS_DEBUG "/W1 /W2 /W3 /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "/O3 /g0 /DNLY_DEBUG")


#----------------------------------------------------
#				       RESOURCE
#----------------------------------------------------
#set(LYCHEE_APP_ICO "${CMAKE_SOURCE_DIR}/resource/images/acaer.rc")


#----------------------------------------------------
#				  SOURCE & INCLUDE
#----------------------------------------------------
# Header fildes
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h
	                           ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*c
                            ${CMAKE_SOURCE_DIR}/src/*.cpp)

### APP ###
add_executable(${PROJECT_NAME} ${SRC_FILES} 
							   ${HEADER_FILES})
							   # ${LYCHEE_APP_ICO})



#----------------------------------------------------
#				    	LIBS
#----------------------------------------------------

#--------RAYLIB--------
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) 	# QUIET or REQUIRED
if (NOT raylib_FOUND) 							# If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) 					# Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()


#----------------------------------------------------
#				      INCLUDE DIRS
#----------------------------------------------------
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
)



#----------------------------------------------------
#				      LINK & PCH
#----------------------------------------------------

#Put all libraries into a variable
set(LIBS raylib)

target_link_libraries(${PROJECT_NAME} ${LIBS})


### PCH ###
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/acaer/ac_pch.h)

	