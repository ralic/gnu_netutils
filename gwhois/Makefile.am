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

bin_PROGRAMS = @whois_BUILD@

EXTRA_PROGRAMS = whois

whois_SOURCES = whois.c

EXTRA_DIST = as_del_list ip_del_list tld_serv_list \
  make_as_del.pl  make_ip_del.pl make_tld_serv.pl \
  as_del.h ip_del.h tld_serv.h

noinst_HEADERS = data.h as_del.h ip_del.h tld_serv.h whois.h
BUILT_SOURCES=as_del.h ip_del.h tld_serv.h

INCLUDES = -I$(top_srcdir)/lib -I../lib 
LDADD = -L../libinetutils -linetutils -L../lib -lgnu


as_del.h: $(srcdir)/as_del_list $(srcdir)/make_as_del.pl
	perl $(srcdir)/make_as_del.pl < $(srcdir)/as_del_list > as_del.h

ip_del.h: $(srcdir)/ip_del_list $(srcdir)/make_ip_del.pl
	perl $(srcdir)/make_ip_del.pl < $(srcdir)/ip_del_list > ip_del.h

tld_serv.h: $(srcdir)/tld_serv_list $(srcdir)/make_tld_serv.pl
	perl $(srcdir)/make_tld_serv.pl < $(srcdir)/tld_serv_list > tld_serv.h
