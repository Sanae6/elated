foreach(v SRCDIR DSTDIR)
  if(NOT DEFINED ${v})
    message(FATAL_ERROR "${v} not defined on command line")
  endif()
endforeach()

file( GLOB_RECURSE pattern_files RELATIVE
  "${SRCDIR}/" "${SRCDIR}/*.*" )
foreach(p IN LISTS pattern_files)
  configure_file(${SRCDIR}/${p}
    ${DSTDIR}/${p} COPYONLY)
endforeach() 
message(STATUS "OK BOOMER")