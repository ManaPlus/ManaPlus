
desktopdir = $(datarootdir)/applications

branding:
	     mkdir -p "$(DESTDIR)$(pkgdatadir)/data/tmw/icons"
	     mkdir -p "$(DESTDIR)$(datarootdir)/icons"
	     mkdir -p "$(DESTDIR)$(desktopdir)"
	     cp tmw.desktop "$(DESTDIR)$(desktopdir)"
	     cp tmw.manaplus "$(DESTDIR)$(pkgdatadir)/data/tmw"
	     cp icons/tmw-client.png "$(DESTDIR)$(pkgdatadir)/data/tmw/icons/tmw.png"
	     cp icons/tmw-client.png "$(DESTDIR)$(datarootdir)/icons/tmw.png"
	     cp -r images "$(DESTDIR)$(pkgdatadir)/data/tmw"

EXTRA_DIST = tmw.desktop.in \
	    tmw.in \
	    tmw.manaplus \
	    icons/tmw-client.ico \
	    icons/tmw-client.png \
	    icons/tmw-client.xpm \
	    images/loginwallpaper_800x600.png
