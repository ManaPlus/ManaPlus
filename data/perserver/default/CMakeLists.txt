SET (FILES
    charcreation.xml
    chatcommands.txt
    deadmessages.xml
    defaultcommands.xml
    features.xml
    gmcommands.txt
    weapons.xml
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/perserver/default)
