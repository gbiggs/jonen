set(srcs tawara_impl.cpp
    vint.cpp
    ebml_int.cpp
    el_ids.cpp
    element.cpp
    int_element.cpp
    uint_element.cpp
    float_element.cpp
    string_element.cpp
    date_element.cpp
    binary_element.cpp
    void_element.cpp
    master_element.cpp
    ebml_element.cpp
    metaseek.cpp
    seek_element.cpp
    segment_info.cpp
    tracks.cpp
    track_entry.cpp
    track_operation.cpp
    block.cpp
    block_impl.cpp
    simple_block.cpp
    block_group.cpp
    block_additions.cpp
    cluster.cpp
    memory_cluster.cpp
    file_cluster.cpp
    segment.cpp
    attachments.cpp
    cues.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

add_library(${PROJECT_NAME_LOWER} ${LIB_TYPE} ${srcs})
target_link_libraries(${PROJECT_NAME_LOWER} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME_LOWER}
    EXPORT ${PROJECT_NAME_LOWER}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT library
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT library
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT library)
install(EXPORT ${PROJECT_NAME_LOWER}
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    FILE ${PROJECT_NAME_LOWER}Depends.cmake)

