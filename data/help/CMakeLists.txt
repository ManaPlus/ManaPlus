SET(FILES
    about.txt
    commands.txt
    chatcommands.txt
    footer.txt
    header.txt
    index.txt
    searchmany.txt
    searchnotfound.txt
    skills.txt
    support.txt
    tags.idx
    team.txt
    windows.txt
    )

ADD_SUBDIRECTORY(tips)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help)
