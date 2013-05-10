
desktopdir = $(datarootdir)/applications

tmw:
	     mkdir -p "$(DESTDIR)$(pkgdatadir)/data/tmw/icons"
	     mkdir -p "$(DESTDIR)$(datarootdir)/icons"
	     mkdir -p "$(DESTDIR)$(desktopdir)"
	     cp tmw.desktop "$(DESTDIR)$(desktopdir)"
	     cp tmw.manaplus "$(DESTDIR)$(pkgdatadir)/data/tmw"
	     cp icons/tmw.png "$(DESTDIR)$(pkgdatadir)/data/tmw/icons"
	     cp icons/tmw.png "$(DESTDIR)$(datarootdir)/icons"
	     cp -r images "$(DESTDIR)$(pkgdatadir)/data/tmw"

EXTRA_DIST = setup_finish.bmp \
	    setup_welcome.bmp \
	    tmw.desktop \
	    tmw.manaplus \
	    icons/tmw.ico \
	    icons/tmw.png \
	    icons/tmw.xpm \
	    images/loginwallpaper_800x600.png
