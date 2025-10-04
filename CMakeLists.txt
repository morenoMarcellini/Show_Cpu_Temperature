cmake_minimum_required(VERSION 3.1.0)
set(PROJECT_NAME showCpuTemperatures)
project(${PROJECT_NAME})

enable_language(C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(HEADER_FILES 
	jetColor.h
	showTemperature.h
	XPixmap.h
	xpm_definition.h
	read_temperature.h
	get_host_name.h
	sync_XAlarm.h
)

set(COMMON_SOURCE
	${CMAKE_SOURCE_DIR}/showTemperature.c
	${CMAKE_SOURCE_DIR}/read_temperature.c
	${CMAKE_SOURCE_DIR}/get_host_name.c
	${CMAKE_SOURCE_DIR}/sync_XAlarm.c
)

set(PROJECT_CONFIG_DIR ${CMAKE_SOURCE_DIR})

add_compile_options(-O2 -pipe -march=native -Wall)
# executable
add_executable(${PROJECT_NAME} ${COMMON_SOURCE})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

# post-build
add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} -lX11 -lXext -lXpm)

