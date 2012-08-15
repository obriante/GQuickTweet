bin_PROGRAMS = gquicktweet
gquicktweet_SOURCES = \
gquicktweet.c


gquicktweet_LDADD = \
	$(EXTERN_LIB_DEP_LIBS) \
	@libnotify_LIBS@ \
	@libtwitc_LIBS@	


if DEBUG

AM_CPPFLAGS = \
	-DDEBUG \
	-D_GNU_SOURCE \
	-DPROG_DIR=\""$(prefix)/share/@PACKAGE@/"\" \
	-DICONS_DIR=\""$(prefix)/share/@PACKAGE@/icons/"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)locale/"\" \
	$(EXTERN_LIB_DEP_CFLAGS) \
	@libnotify_CFLAGS@ \
	@libtwitc_CFLAGS@ \
	@GQuickTweet_CFLAGS@
else

AM_CPPFLAGS = \
	-D_GNU_SOURCE \
	-DPROG_DIR=\""$(prefix)/share/@PACKAGE@/"\" \
	-DICONS_DIR=\""$(prefix)/share/@PACKAGE@/icons/"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)locale/"\" \
	$(EXTERN_LIB_DEP_CFLAGS) \
	@libnotify_CFLAGS@ \
	@libtwitc_CFLAGS@ \
	@GQuickTweet_CFLAGS@

endif