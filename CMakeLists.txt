project (nanostate) 
cmake_minimum_required (VERSION 2.8.0) 

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message (FATEL_ERROR "In-source builds are not allowed")
endif ()

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_MODULE_PATH 
  ${PROJECT_SOURCE_DIR}/cmake-modules
  )
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

option (ENABLE_QT "Enable QT" OFF)
option (ENABLE_QT_EXAMPLES "Enable QT Examples" OFF)

include_directories (
  ${PROJECT_SOURCE_DIR}
)

link_directories (
  ${PROJECT_BINARY_DIR}/lib
)

subdirs (libnanostate examples)

if (ENABLE_QT) 
  subdirs (libqnanostate)
  set (CMAKE_AUTOMOC TRUE)
  if (ENABLE_QT_EXAMPLES)
    find_package (Qt4 REQUIRED COMPONENTS QtCore QtGui QtOpenGL)
  else ()
    find_package (Qt4 REQUIRED COMPONENTS QtCore)
  endif ()
  include (${QT_USE_FILE})
endif ()

