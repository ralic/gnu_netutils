/* printif.h

   Copyright (C) 2001 Free Software Foundation, Inc.

   Written by Marcus Brinkmann.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef IFCONFIG_PRINTIF_H
#define IFCONFIG_PRINTIF_H

#include <net/if.h>
#include <arpa/inet.h>
#include "ifconfig.h"

/* The column position of the cursor.  */
extern int *column;

/* Whenever you output something, set this to true.  */
extern int had_output;

struct format_data
{
  char *name;	/* Name of interface as specified on the command line.  */
  struct ifreq *ifr;
  int sfd;	/* Socket file descriptor to use.  */
  int first;	/* This is the first interface.  */
  char *format;	/* The format string.  */
  int depth;	/* Depth of nesting in parsing.  */
};

typedef struct format_data *format_data_t;

typedef void (*format_handler_t)(format_data_t, int, char **);

struct format_handle
{
  char *name;	/* The name of the handler.  */
  format_handler_t handler;
};

extern struct format_handle format_handles[];

/* Each TAB_STOP characters is a default tab stop, which is also used
   by '\t'.  */
#define TAB_STOP 8

void put_char (format_data_t form, char c);
void put_string (format_data_t form, char *s);
void put_int (format_data_t form, int argc, char *argv[], int nr);
void select_arg (format_data_t form, int argc, char *argv[], int nr);
void put_addr (format_data_t form, int argc, char *argv[], struct sockaddr *sa);
void put_flags (format_data_t form, int argc, char *argv[], short flags);

/* Format handler can mangle form->format, so update it after calling
   here.  */
void format_handler (char *name, format_data_t form, int argc, char *argv[]);

void fh_nothing (format_data_t form, int argc, char *argv[]);
void fh_newline (format_data_t form, int argc, char *argv[]);
void fh_tabulator (format_data_t form, int argc, char *argv[]);
void fh_first (format_data_t form, int argc, char *argv[]);
void fh_tab (format_data_t form, int argc, char *argv[]);
void fh_join (format_data_t form, int argc, char *argv[]);
void fh_exists_query (format_data_t form, int argc, char *argv[]);
void fh_format (format_data_t form, int argc, char *argv[]);
void fh_error (format_data_t form, int argc, char *argv[]);
void fh_progname (format_data_t form, int argc, char *argv[]);
void fh_exit (format_data_t form, int argc, char *argv[]);
void fh_name (format_data_t form, int argc, char *argv[]);
void fh_index_query (format_data_t form, int argc, char *argv[]);
void fh_index (format_data_t form, int argc, char *argv[]);
void fh_addr_query (format_data_t form, int argc, char *argv[]);
void fh_addr (format_data_t form, int argc, char *argv[]);
void fh_netmask_query (format_data_t form, int argc, char *argv[]);
void fh_netmask (format_data_t form, int argc, char *argv[]);
void fh_brdaddr_query (format_data_t form, int argc, char *argv[]);
void fh_brdaddr (format_data_t form, int argc, char *argv[]);
void fh_dstaddr_query (format_data_t form, int argc, char *argv[]);
void fh_dstaddr (format_data_t form, int argc, char *argv[]);
void fh_flags_query (format_data_t form, int argc, char *argv[]);
void fh_flags (format_data_t form, int argc, char *argv[]);
void fh_mtu_query (format_data_t form, int argc, char *argv[]);
void fh_mtu (format_data_t form, int argc, char *argv[]);
void fh_metric_query (format_data_t form, int argc, char *argv[]);
void fh_metric (format_data_t form, int argc, char *argv[]);

/* Used for recursion by format handlers.  */
void print_interfaceX (format_data_t form, int quiet);

void print_interface (int sfd, char *name, struct ifreq *ifr, char *format);

#endif
