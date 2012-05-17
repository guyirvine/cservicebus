#ifndef _HANDLERMGR_H
#define _HANDLERMGR_H 1

#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <dlfcn.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>

#include <apr_general.h>


struct _handler {
  char *path;
  void *handle;
  time_t mtime;
  char **supports;
  int support_count;
  int initialised;
};


int load_handlers( char *base_dir, struct _handler ***handler_list, int handler_count );

int reload_handlers( char *base_dir, struct _handler ***handler_list, int handler_count );

#endif
