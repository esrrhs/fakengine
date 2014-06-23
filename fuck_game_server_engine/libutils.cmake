
# Merge static libraries into a big static lib. The resulting library 
# should not not have dependencies on other static libraries.
# We use it in MySQL to merge mysys,dbug,vio etc into mysqlclient

MACRO(MERGE_STATIC_LIBS TARGET OUTPUT_NAME STATIC_LIBS)
  # To produce a library we need at least one source file.
  # It is created by ADD_CUSTOM_COMMAND below and will
  # also help to track dependencies.
  SET(SOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_depends.c)
  ADD_LIBRARY(${TARGET} STATIC ${SOURCE_FILE})
  SET_TARGET_PROPERTIES(${TARGET} PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

  MESSAGE("TARGET ${TARGET} OUTPUT_NAME ${OUTPUT_NAME} CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} STATIC_LIBS ${STATIC_LIBS} CMAKE_COMMAND ${CMAKE_COMMAND}")

  TARGET_LINK_LIBRARIES(${TARGET} pthread dl rt)
	
  # Make the generated dummy source file depended on all static input
  # libs. If input lib changes,the source file is touched
  # which causes the desired effect (relink).
  ADD_CUSTOM_COMMAND( 
    OUTPUT  ${SOURCE_FILE}
    COMMAND ${CMAKE_COMMAND}  -E touch ${SOURCE_FILE}
    DEPENDS ${STATIC_LIBS})

    GET_TARGET_PROPERTY(TARGET_LOCATION ${TARGET} LOCATION)  
	
    MESSAGE("TARGET_LOCATION ${TARGET_LOCATION} STATIC_LIBS ${STATIC_LIBS}")
  
      # Generic Unix, Cygwin or MinGW. In post-build step, call
      # script, that extracts objects from archives with "ar x" 
      # and repacks them with "ar r"
      SET(TARGET ${TARGET})
      CONFIGURE_FILE(
        ./merge_archives_unix.cmake.in
        ./merge_archives_${TARGET}.cmake 
        @ONLY
      )
	  MESSAGE("CONFIGURE_FILE ok")
      ADD_CUSTOM_COMMAND(TARGET ${TARGET} POST_BUILD
        COMMAND rm ${TARGET_LOCATION}
        COMMAND ${CMAKE_COMMAND} -P 
        ${CMAKE_CURRENT_BINARY_DIR}/merge_archives_${TARGET}.cmake
      )
	  MESSAGE("ADD_CUSTOM_COMMAND ${TARGET_LOCATION} ${CMAKE_COMMAND} merge_archives_${TARGET}.cmake ok")

ENDMACRO()
