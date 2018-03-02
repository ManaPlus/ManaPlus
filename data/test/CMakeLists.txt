SET(FILES
    arrow_up.png
    arrow_up_A.png
    arrow_up_B.png
    arrow_up_S.png
    dye.png
    equipmentwindow.xml
    hide.png
    items.xml
    itemxmlutils_test.xml
    palette.gpl
    paths.xml
    quests.xml
    serverlistplus.xml
    simplefile.txt
    test.txt
    test2.txt
    test.zip
    test2.zip
    test3.zip
    test4.zip
    test5.zip
    testintmap.xml
    units.xml
    graphics/sprites/hairstyles/hairstyle01.png
    graphics/sprites/hairstyles/hairstyle01.xml
    graphics/sprites/hairstyles/hairstyle02.png
    graphics/sprites/hairstyles/hairstyle02.xml
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/test)

ADD_SUBDIRECTORY(dir1 dir2)
