# Copyright (C) 2005, 2007 Free Software Foundation, Inc.
# This file is part of Netutils.
#
# Netutils is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# Netutils is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Netutils; see the file COPYING.  If not, write
# to the Free Software Foundation, Inc., 51 Franklin Street,
# Fifth Floor, Boston, MA 02110-1301 USA.

noinst_LTLIBRARIES = libinetutils.la

noinst_HEADERS = argcv.h libinetutils.h

EXTRA_DIST = logwtmp.c

libinetutils_la_SOURCES = argcv.c cleansess.c daemon.c des_rw.c kcmd.c	\
	krcmd.c localhost.c logwtmpko.c setsig.c shishi.c tftpsubs.c \
        ttymsg.c utmp_init.c utmp_logout.c

libinetutils_la_LIBADD = daemon.o -L$(top_builddir)/lib -lgnu

@PATHDEFS_MAKE@

AM_CPPFLAGS = $(PATHDEF_WTMPX) $(PATHDEF_TTY_PFX) $(PATHDEF_DEVNULL) $(PATHDEF_HEQUIV) 

INCLUDES = -I$(top_srcdir)/headers -I$(top_srcdir)/lib
