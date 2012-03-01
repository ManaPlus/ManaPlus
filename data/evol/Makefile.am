
desktopdir = $(datarootdir)/applications

evol-client:
	     mkdir -p "$(pkgdatadir)/data/evol/icons"
	     cp evol.desktop "$(desktopdir)"
	     cp evol.manaplus "$(pkgdatadir)/data/evol"
	     cp icons/evol-client.png "$(pkgdatadir)/data/evol/icons"
	     cp icons/evol-client.png "$(datarootdir)/icons"
	     cp -r images "$(pkgdatadir)/data/evol"

EXTRA_DIST = evol.desktop \
	    evol.manaplus \
	    icons/evol-client.ico \
	    icons/evol-client.png \
	    icons/evol-client.xpm \
	    images/loginwallpaper_800x600.png
