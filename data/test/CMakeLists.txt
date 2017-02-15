SET(FILES
    arrow_up.png
    arrow_up_A.png
    arrow_up_B.png
    arrow_up_S.png
    dye.png
    equipmentwindow.xml
    hide.png
    items.xml
    palette.gpl
    paths.xml
    quests.xml
    serverlistplus.xml
    simplefile.txt
    test.txt
    test2.txt
    test.zip
    testintmap.xml
    units.xml
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/test)

ADD_SUBDIRECTORY(dir1)
