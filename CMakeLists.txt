cmake_minimum_required(VERSION 3.24.1)

#----------------------------------------------------
#			   	 PROJECT PROPERTIES
#----------------------------------------------------
project(acaer VERSION 0.0.2)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

## Requierments ## 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

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
set(ACAER_APP_ICO "${CMAKE_SOURCE_DIR}/resource/images/acaer_logo.rc")

#----------------------------------------------------
#				  SOURCE & INCLUDE
#----------------------------------------------------
# Global Header fildes
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h
	                           ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Global Source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*c
                            ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE IMGUI_FILES ${CMAKE_SOURCE_DIR}/lib/imgui-sfml/*.h
							  ${CMAKE_SOURCE_DIR}/lib/imgui-sfml/*.cpp)


### APP ###
add_executable(${PROJECT_NAME} 	${SRC_FILES} 
							   	${HEADER_FILES}
							   	${IMGUI_FILES}
							   	${ACAER_APP_ICO})


#----------------------------------------------------
#				    	LIBS
#----------------------------------------------------

#---------SFML---------
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR ${CMAKE_SOURCE_DIR}/lib/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML 2.5.1 REQUIRED COMPONENTS window system graphics audio network)

#---------BOX2D---------
set(BOX2D_LIB ${CMAKE_SOURCE_DIR}/lib/box2d/lib/box2d.lib)

#--------YAML--------
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/vendor/yaml/)

#----------------------------------------------------
#				      INCLUDE DIRS
#----------------------------------------------------
include_directories(
	${CMAKE_SOURCE_DIR}/src
  	#EnTT
	${CMAKE_SOURCE_DIR}/lib/vendor/entt
	# spdlog
	${CMAKE_SOURCE_DIR}/lib/vendor/spdlog/include
	#SFML
	${CMAKE_SOURCE_DIR}/lib/SFML-2.5.1/include
	# Box2D
	${CMAKE_SOURCE_DIR}/lib/box2d/include
	#ImGui
	${CMAKE_SOURCE_DIR}/lib/imgui-sfml/include
)



#----------------------------------------------------
#				      LINK & PCH
#----------------------------------------------------

#Put all libraries into a variable
set(LIBS 	sfml-window
			sfml-system
			sfml-graphics
			sfml-audio
			sfml-network
			yaml-cpp
			${BOX2D_LIB})

target_link_libraries(${PROJECT_NAME} ${LIBS})


### PCH ###
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/acaer/ac_pch.h)

	