SET(FILES
    gles2_frag.glsl
    gles2_vertex.glsl
    simple_frag.glsl
    simple_vertex.glsl
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/graphics/shaders)
