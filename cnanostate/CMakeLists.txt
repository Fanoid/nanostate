find_package (MsgPack REQUIRED)
find_package (nanomsg REQUIRED)

include_directories (
  ${NANOMSG_INCLUDE_DIR}
)

set (nanostate_sources 
  nanostate.cpp
) 
add_library (nanostate ${nanostate_sources})
target_link_libraries (nanostate ${NANOMSG_LIBRARY} ${MSGPACK_LIBRARIES})

add_executable (hub hub.c) 
target_link_libraries (hub ${NANOMSG_LIBRARY})
