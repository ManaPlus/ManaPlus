SET (FILES
    charcreation.xml
    chatcommands.txt
    deadmessages.xml
    defaultcommands.xml
    features.xml
    gmcommands.txt
    groups.xml
    weapons.xml
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/perserver/default)
