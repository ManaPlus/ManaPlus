SET(FILES
    about.txt
    commands.txt
    chatcommands.txt
    header.txt
    index.txt
    skills.txt
    support.txt
    team.txt
    teamtmw.txt
    windows.txt
    )

ADD_SUBDIRECTORY(ru_RU)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help)
