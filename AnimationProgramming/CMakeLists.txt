set(EXE_NAME AnimProgExe)
set(ENGINE_LIB ${PROJECT_SOURCE_DIR}/AnimationProgramming/Data/Engine.lib)

set(ENGINE_DLL ${PROJECT_SOURCE_DIR}/AnimationProgramming/Data/Engine.dll)
set(GLEW_DLL ${PROJECT_SOURCE_DIR}/AnimationProgramming/Data/glew32.dll)
set(SDL2 ${PROJECT_SOURCE_DIR}/AnimationProgramming/Data/SDL2.dll)
set(LIBMATHS_LIB ${PROJECT_SOURCE_DIR}/build/libMaths/libMaths/Debug/libMaths.lib)

set(DEST_DIR ${PROJECT_SOURCE_DIR}/build/AnimationProgramming)
set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/AnimationProgramming/Data/Resources)
set(RESOURCES_LIST ${PROJECT_SOURCE_DIR}/AnimationProgramming/Data/Resources.list)


include(CMake/CMakeLists_sources.cmake)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${${EXE_NAME}_SOURCES})
message( ${${EXE_NAME}_SOURCES} )

file(COPY  ${RESOURCES_DIR} DESTINATION  ${DEST_DIR})

configure_file(${RESOURCES_LIST} ${DEST_DIR} COPYONLY)

add_executable(${EXE_NAME} ${${EXE_NAME}_SOURCES})

target_link_libraries(${EXE_NAME} PRIVATE ${ENGINE_LIB}
					  ${LIBMATHS_LIB})

target_include_directories(${EXE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/libMaths/libMaths/source)



add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ENGINE_DLL} $<TARGET_FILE_DIR:${EXE_NAME}>)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_DLL} $<TARGET_FILE_DIR:${EXE_NAME}>)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2} $<TARGET_FILE_DIR:${EXE_NAME}>)
