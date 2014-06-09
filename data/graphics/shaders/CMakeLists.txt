SET(FILES
    simple_frag.glsl
    simple_vertex.glsl
    texture_frag.glsl
    texture_vertex.glsl
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/graphics/shaders)
