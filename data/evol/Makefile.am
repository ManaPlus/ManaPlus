
desktopdir = $(datarootdir)/applications

branding:
	     mkdir -p "$(DESTDIR)$(pkgdatadir)/data/evol/icons"
	     mkdir -p "$(DESTDIR)$(datarootdir)/icons"
	     mkdir -p "$(DESTDIR)$(desktopdir)"
	     cp evol.desktop "$(DESTDIR)$(desktopdir)"
	     cp evol.manaplus "$(DESTDIR)$(pkgdatadir)/data/evol"
	     cp icons/evol-client.png "$(DESTDIR)$(pkgdatadir)/data/evol/icons"
	     cp icons/evol-client.png "$(DESTDIR)$(datarootdir)/icons"
	     cp -r images "$(DESTDIR)$(pkgdatadir)/data/evol"

EXTRA_DIST = evol.desktop.in \
	    evol.in \
	    evol.manaplus \
	    icons/evol-client.ico \
	    icons/evol-client.png \
	    icons/evol-client.xpm \
	    images/loginwallpaper_800x600.png
