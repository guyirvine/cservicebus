#ifndef _HANDLER_H
#define _HANDLER_H 1

#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <dlfcn.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>

#include <apr_general.h>

#include "libhandlermgr.c"

struct _handler_parameter {
	apr_pool_t *pool;
	void *data;
};

int handle_message( apr_pool_t *pool, void *handle, void *data );

int bus_handle( apr_pool_t *pool, struct _handler **handler_list, int handler_count, char *msg, int msg_size, int *handled );

#endif
