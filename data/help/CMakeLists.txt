SET(FILES
    about.txt
    chatcommands.txt
    chatguild.txt
    chatparty.txt
    commands.txt
    download.txt
    footer.txt
    header.txt
    index.txt
    otherteam.txt
    searchmany.txt
    searchnotfound.txt
    skills.txt
    support.txt
    team.txt
    windows.txt
    )

ADD_SUBDIRECTORY(idx tips)

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help)
