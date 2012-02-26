
iconsdir = $(pkgdatadir)/data/icons

icons_DATA = manaplus.png \
	     manaplus.icns \
	     manaplus.ico \
	     manaplus.hqx \
	     manaplus.xpm

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datarootdir)/pixmaps
	$(INSTALL) $(srcdir)/manaplus.png $(DESTDIR)$(datarootdir)/pixmaps/manaplus.png

uninstall-local:
	rm -f $(DESTDIR)$(datarootdir)/pixmaps/manaplus.png

EXTRA_DIST =		\
	$(icons_DATA) \
	CMakeLists.txt
