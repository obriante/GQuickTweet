AC_PREREQ(2.59)
AC_INIT(GQuickTweet, 1.0)
AC_PROG_CC
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([no])])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall])

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O 

dnl Doxygen
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(ON)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(ON)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([$PACKAGE_NAME],[GQuickTweet.doxyfile],[doc])

PKG_CHECK_MODULES(EXTERN_LIB_DEP, [libcurl libxml-2.0 oauth gtk+-2.0])

PKG_CHECK_MODULES(libnotify, [libnotify >= 0.7.3], , [
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([

You must have libnotify >= 0.7.3
])])

AC_SUBST(libnotify_CFLAGS)
AC_SUBST(libnotify_LIBS)


AC_CONFIG_SUBDIRS( libtwitc )
	libtwitc_CFLAGS="-I\$(top_srcdir)/libtwitc/include"
	libtwitc_LIBS="\$(top_builddir)/libtwitc/src/libtwitc.a"
AC_SUBST(libtwitc_CFLAGS)
AC_SUBST(libtwitc_LIBS)

AC_CONFIG_SUBDIRS( libuigtk )
	libuigtk_CFLAGS="-I\$(top_srcdir)/libuigtk/include"
	libuigtk_LIBS="\$(top_builddir)/libuigtk/src/libuigtk.a"
AC_SUBST(libuigtk_CFLAGS)
AC_SUBST(libuigtk_LIBS)

AC_CONFIG_SUBDIRS( GQuickTweet )
	GQuickTweet_CFLAGS="-I\$(top_srcdir)/include"
AC_SUBST(GQuickTweet_CFLAGS)

dnl DEBUG
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac], [debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")


AC_PROG_RANLIB

AC_CONFIG_FILES([
libui_gtk/Makefile
src/Makefile
Makefile
])
AC_OUTPUT