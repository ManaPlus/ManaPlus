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

ADD_SUBDIRECTORY(es)
ADD_SUBDIRECTORY(fr)
ADD_SUBDIRECTORY(ru)
ADD_SUBDIRECTORY(pt)
ADD_SUBDIRECTORY(pt_BR)
ADD_SUBDIRECTORY(tips)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help)
