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
ADD_SUBDIRECTORY(fr_FR)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help/fr_FR)