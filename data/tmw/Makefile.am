
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

EXTRA_DIST = tmw.desktop \
	    tmw.manaplus \
	    icons/tmw-client.ico \
	    icons/tmw-client.png \
	    icons/tmw-client.xpm \
	    images/loginwallpaper_800x600.png
