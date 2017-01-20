SET(FILES
    DejaVuSans-Bold.sfd
    DejaVuSans.sfd
    DejaVuSansMono-Bold.sfd
    DejaVuSansMono.sfd
    DejaVuSerifCondensed-Bold.sfd
    DejaVuSerifCondensed.sfd
    LiberationMono-Bold.sfd
    LiberationMono-Regular.sfd
    LiberationSans-Bold.sfd
    LiberationSans-Regular.sfd
    into.txt
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/fonts/src)
