/* Copyright (C) 2008 Free Software Foundation, Inc.

   This file is part of Netutils.

   Netutils is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Netutils is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Netutils; see the file COPYING.  If not, write
   to the Free Software Foundation, Inc., 51 Franklin Street,
   Fifth Floor, Boston, MA 02110-1301 USA. */

#include <config.h>

#include <argp.h>
#include <error.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "libinetutils.h"
#include "progname.h"
#include "xalloc.h"

struct _hostname_options
{
    const char *hostname_file;
    const char *hostname_new;
    short int hostname_alias;
    short int hostname_fqdn;
    short int hostname_ip_address;
    short int hostname_dns_domain;
    short int hostname_short;
};
typedef struct _hostname_options hostname_options;

typedef int (*get_name_action_t) (char *name, size_t size);
typedef int (*set_name_action_t) (const char *name, size_t size);

static get_name_action_t get_name_action = NULL;
static set_name_action_t set_name_action = NULL;

ARGP_PROGRAM_DATA ("hostname", "2008", "Debarshi Ray");

const char args_doc[] = "[NAME]";
const char doc[] = "Show or set the system's host name.";

static struct argp_option argp_options[] = {
#define GRP 0
  {"aliases", 'a', NULL, 0, "Alias names", GRP+1},
  {"alias", 'a', NULL, OPTION_ALIAS, "Alias names", GRP+1},
  {"domain", 'd', NULL, 0, "DNS domain name", GRP+1},
  {"file", 'F', "FILE", 0, "Read host name or NIS domain name "
   "from FILE", GRP+1},
  {"fqdn", 'f', NULL, 0, "DNS host name or FQDN", GRP+1},
  {"long", 'f', NULL, OPTION_ALIAS, "DNS host name or FQDN", GRP+1},
  {"ip-addresses", 'i', NULL, 0, "Addresses for the host name", GRP+1},
  {"ip-address", 'i', NULL, OPTION_ALIAS, "Addresses for the host name",
   GRP+1},
  {"short", 's', NULL, 0, "Short host name", GRP+1},
  {"yp", 'y', NULL, 0, "NIS/YP domain name", GRP+1},
  {"nis", 'y', NULL, OPTION_ALIAS, "NIS/YP domain name", GRP+1},
#undef GRP
  {NULL}
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  hostname_options *const options = (hostname_options *const) state->input;

  switch (key)
    {
    case 'a':
      get_name_action = gethostname;
      options->hostname_alias = 1;
      break;

    case 'd':
      get_name_action = gethostname;
      options->hostname_fqdn = 1;
      options->hostname_dns_domain = 1;
      break;

    case 'F':
      set_name_action = (set_name_action_t) sethostname;
      options->hostname_file = arg;
      break;

    case 'f':
      get_name_action = gethostname;
      options->hostname_fqdn = 1;
      break;

    case 'i':
      get_name_action = gethostname;
      options->hostname_ip_address = 1;
      break;

    case 's':
      get_name_action = gethostname;
      options->hostname_fqdn = 1;
      options->hostname_short = 1;
      break;

    case 'y':
      get_name_action = (get_name_action_t) getdomainname;
      break;

    case ARGP_KEY_ARG:
      set_name_action = (set_name_action_t) sethostname;
      options->hostname_new = strdup (arg);
      if (options->hostname_new == NULL)
        error (EXIT_FAILURE, errno, "strdup");
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

static struct argp argp = {argp_options, parse_opt, args_doc, doc};

static void get_name (const hostname_options *const options);
static void set_name (const hostname_options *const options);
static char * get_aliases (const char *const host_name);
static char * get_fqdn (const char *const host_name);
static char * get_ip_addresses (const char *const host_name);
static char * get_dns_domain_name (const char *const host_name);
static char * get_short_hostname (const char *const host_name);
static char * parse_file (const char *const file_name);

int
main (int argc, char *argv[])
{
  hostname_options options;

  memset ((void *) &options, 0, sizeof (options));
  set_program_name (argv[0]);

  /* Parse command line */
  argp_parse (&argp, argc, argv, 0, NULL, (void *) &options);

  /* Set default action */
  if (get_name_action == NULL && set_name_action ==  NULL)
    get_name_action = gethostname;

  if (get_name_action == (get_name_action_t) getdomainname
      || get_name_action == gethostname)
    get_name (&options);
  else if (set_name_action == (set_name_action_t) sethostname)
    set_name (&options);

  return 0;
}

static void
get_name (const hostname_options *const options)
{
  char *buffer;
  char *name;
  int status;
  size_t size;

  size = sysconf (_SC_HOST_NAME_MAX);
  buffer = (char *) xmalloc (sizeof (char) * (size + 1));

  status = (*get_name_action) (buffer, size);
  while (status == -1)
    {
      size += 32;
      buffer = (char *) xrealloc (buffer, size);
      status = (*get_name_action) (buffer, size);
    }

  if (options->hostname_alias == 1)
      name = get_aliases (buffer);
  else if (options->hostname_fqdn == 1)
    {
      name = get_fqdn (buffer);

      if (options->hostname_dns_domain == 1 || options->hostname_short == 1)
        {
          free (buffer);
          buffer = name;
          name = NULL;
        }

      if (options->hostname_dns_domain == 1)
        name = get_dns_domain_name (buffer);
      else if (options->hostname_short == 1)
        name = get_short_hostname (buffer);
    }
  else if (options->hostname_ip_address == 1)
      name = get_ip_addresses (buffer);
  else
    {
      name = strdup (buffer);
      if (name == NULL)
        error (EXIT_FAILURE, errno, "strdup");
    }

  puts (name);

  free (name);
  free (buffer);
  return;
}

static void
set_name (const hostname_options *const options)
{
  const char *hostname_new;
  int status;
  size_t size;

  if (options->hostname_file != NULL)
    hostname_new = parse_file (options->hostname_file);
  else
    hostname_new = options->hostname_new;

  size = strlen (hostname_new);
  status = (*set_name_action) (hostname_new, size);
  if (status == -1)
    error (EXIT_FAILURE, errno, "sethostname");

  free ((void *) hostname_new);
  return;
}

static char *
get_aliases (const char *const host_name)
{
  char *aliases;
  unsigned int count = 0;
  unsigned int i;
  unsigned int size = 256;
  struct hostent *ht;

  aliases = (char *) xmalloc (sizeof (char) * size);
  aliases[0] = '\0';

  ht = gethostbyname (host_name);
  if (ht == NULL)
    strcpy (aliases, "(none)");
  else
    {
      for (i = 0; ht->h_aliases[i] != NULL; i++)
        {
          /* Aliases should be blankspace separated. */
          if (ht->h_aliases[i+1] != NULL)
            count++;
          count += strlen (ht->h_aliases[i]);
          if (count >= size)
            {
              size *= 2;
              aliases = xrealloc (aliases, size);
            }

          strcat (aliases, ht->h_aliases[i]);
          strcat (aliases, " ");
        }
    }

  return aliases;
}

static char *
get_fqdn (const char *const host_name)
{
  char *fqdn;
  struct hostent *ht;

  ht = gethostbyname (host_name);
  if (ht == NULL)
    fqdn = strdup ("(none)");
  else
    fqdn = strdup (ht->h_name);

  if (fqdn == NULL)
    error (EXIT_FAILURE, errno, "strdup");

  return fqdn;
}

static char *
get_ip_addresses (const char *const host_name)
{
  char address[16];
  char *addresses;
  unsigned int count = 0;
  unsigned int i;
  unsigned int size = 256;
  struct hostent *ht;

  addresses = (char *) xmalloc (sizeof (char) * size);
  addresses[0] = '\0';

  ht = gethostbyname (host_name);
  if (ht == NULL)
    strcpy (addresses, "(none)");
  else
    {
      for (i = 0; ht->h_addr_list[i] != NULL; i++)
        {
          inet_ntop (ht->h_addrtype, (void *) ht->h_addr_list[i],
                     address, sizeof (address));

          /* IP addresses should be blankspace separated. */
          if (ht->h_addr_list[i+1] != NULL)
            count++;
          count += strlen (address);
          if (count >= size)
            {
              size *= 2;
              addresses = xrealloc (addresses, size);
            }

          strcat (addresses, address);
          strcat (addresses, " ");
        }
    }

  return addresses;
}

static char *
get_dns_domain_name (const char *const host_name)
{
  char *domain_name;
  const char * pos;

  pos = strchr (host_name, '.');
  if (pos == NULL)
    domain_name = strdup ("(none)");
  else
    domain_name = strdup (pos+1);

  if (domain_name == NULL)
    error (EXIT_FAILURE, errno, "strdup");

  return domain_name;
}

static char *
get_short_hostname (const char *const host_name)
{
  size_t size;
  char *short_hostname;
  const char * pos;

  pos = strchr (host_name, '.');
  if (pos == NULL)
    short_hostname = strdup (host_name);
  else
    {
      size = pos - host_name;
      short_hostname = strndup (host_name, size);
    }

  if (short_hostname == NULL)
    error (EXIT_FAILURE, errno, "strdup");

  return short_hostname;
}

static char *
parse_file (const char *const file_name)
{
  char *buffer = NULL;
  char *name;
  FILE *file;
  size_t nread;
  size_t size = 0;

  file = fopen (file_name, "r");
  if (file == NULL)
    error (EXIT_FAILURE, errno, "fopen");

  nread = getline (&buffer, &size, file);
  if (nread == -1)
    error (EXIT_FAILURE, errno, "getline");

  while (feof (file) == 0)
    {
      if (buffer[0] != '#')
        {
          name = xmalloc (sizeof (char) * nread);
          sscanf (buffer, "%s", name);
          break;
        }

      nread = getline (&buffer, &size, file);
      if (nread == -1)
        error (EXIT_FAILURE, errno, "getline");
    }

  free (buffer);
  fclose (file);
  return name;
}
