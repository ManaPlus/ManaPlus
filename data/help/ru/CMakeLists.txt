SET(FILES
    chatcommands.txt
    header.txt
    index.txt
    team.txt
    )

ADD_SUBDIRECTORY(tips)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help/ru)
