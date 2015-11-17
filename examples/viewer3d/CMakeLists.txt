find_package (Protobuf REQUIRED)

include_directories (
  ${PROTOBUF_INCLUDE_DIRS}
  )

set (sources 
  trackball.cpp
  widget.cpp
  main.cpp
  ) 

set (protos trackball.proto) 
protobuf_generate_cpp (proto_sources proto_headers ${protos}) 

add_executable (viewer3d ${sources} ${proto_sources}) 
target_link_libraries (viewer3d
  qnanostate
  ${QT_LIBRARIES}
  ${PROTOBUF_LIBRARIES}
  GL GLU glut
  )
