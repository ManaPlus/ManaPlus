
iconsdir = $(pkgdatadir)/data/icons

icons_DATA = manaplus.png \
	     manaplus.svg \
	     manaplus.icns \
	     manaplus.ico \
	     manaplus.hqx \
	     manaplus.xpm

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps
	$(INSTALL) $(srcdir)/manaplus.svg $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps/manaplus.svg

uninstall-local:
	rm -f $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps/manaplus.svg

EXTRA_DIST =		\
	$(icons_DATA) \
	CMakeLists.txt
