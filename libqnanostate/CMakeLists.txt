set (qnanostate_src
  qnanostate.cpp
) 

add_library (qnanostate ${qnanostate_src}) 
target_link_libraries (qnanostate nanostate ${QT_LIBRARIES}) 
