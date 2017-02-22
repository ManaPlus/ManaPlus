SET(FILES
    file1.txt
    file2.txt
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/test/dir2)
