SET(FILES
    about.txt
    commands.txt
    chatcommands.txt
    header.txt
    index.txt
    skills.txt
    support.txt
    team.txt
    windows.txt
    )

ADD_SUBDIRECTORY(tips)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help)
