#include "libhandlermgr.h"


struct _handler *load_handler( char *path ) {
	void *handle;
	struct _handler *handler;
	int (*supports)( char ***list );


	syslog( LOG_ERR, "Open the handler: %s", path);
	handle = dlopen (path, RTLD_LAZY);
	if (!handle) {
		syslog( LOG_ERR, "Handler: %s, failed to load. Error: %s.\n", path, dlerror());
		return NULL;
	}
	dlerror();    /* Clear any existing error */


	syslog( LOG_ERR, "Make sure it has the supports function");
	supports = dlsym(handle, "supports");
	if (supports == NULL)  {
		syslog( LOG_ERR, "Handler: %s, does not have 'supports' function.\n", path);
		return NULL;
	}


	syslog( LOG_ERR, "Load all the info we need about the handler");
	handler = ( struct _handler * ) malloc( sizeof( struct _handler ) );
	handler->path = strdup( path );
	handler->handle = handle;
	handler->initialised = 0;	
	handler->support_count = (*supports)( &handler->supports );


	syslog( LOG_NOTICE, "Handler Loaded: %s\n", path);
	return handler;
}


int add_handler( struct _handler ***outbound_list, int count, char *path ) {
	int reload = 0;


	syslog( LOG_ERR,  "Loading handler: %s\n", path);
	struct _handler *handler = load_handler( path );


	if ( handler == NULL ) {
		syslog( LOG_ERR,  "Handler failed to load: %s\n", path);
		return count;
	}


	syslog( LOG_ERR,  "Handler loaded, adding to list\n");
	{
		struct _handler **list = *outbound_list;
		int n = count;
		while( n-- ) {
			if ( list[n] == NULL ) {
				list[n] = handler;
				*outbound_list = list;
				return count;
			}
		}


		syslog( LOG_ERR,  "Add Handler to end of list.\n");
		n = count + 1;
		list = realloc( list, sizeof( struct _handler *) * n );
		list[count] = handler;

		*outbound_list = list;
		return n;
	}
}


int handler_filter(const struct dirent *c) {
	if ( !fnmatch( "*so", c->d_name, 0 ) ) {
		return 1;
	} else {
		return 0;
	}
}

int load_handlers_from_dir( char *base_dir, struct _handler ***outbound_list, int count ) {
	struct dirent **namelist;


	syslog( LOG_ERR, "Scan dir for handlers: %s", base_dir);
	int filecount = scandir( base_dir, &namelist, handler_filter, NULL);
	if (filecount < 0) {
		syslog( LOG_ERR, "An error occured scanning: %s, for handlers", base_dir );
		return count;
	} else if ( filecount == 0 ) {
		syslog( LOG_ERR, "No handlers found in: %s", base_dir );
		return count;
	}


	syslog( LOG_ERR, "At least one handler found, so process what we found");
	struct _handler **list = *outbound_list;
	int n;
	for( n=0;n<filecount;n++ ) {
		char path[2048];
		path[0] = '\0';
		strcat(path, base_dir );
		strcat(path, namelist[n]->d_name );
		count = add_handler( &list, count, path );

		free(namelist[n]);
	}
	free(namelist);

	
	*outbound_list = list;
	return count;
}

void init_handlers( struct _handler **list, int count ) {
	int n = 0;


	for(n=0;n<count;n++) {
		if ( list[n] != NULL && list[n]->initialised == 0 ) {
//			init_handler( list[n]->handle );
/*
    void (*init_db)( PGconn* conn );
    void (*init_mq)( void* conn );

    init_db = dlsym(handle, "init_db");
    if (init_db != NULL)  {
      (*init_db)( conn );
    }

    init_mq = dlsym(handle, "init_mq");
    if (init_mq != NULL)  {
      (*init_db)( conn );
    }
*/
			
			list[n]->initialised =1;
		}
	}
}


int load_handlers( char *base_dir, struct _handler ***list, int count ) {
	int new_count = 0;

	new_count = load_handlers_from_dir( base_dir, list, count );
	init_handlers( (*list), new_count );

	return new_count;
}


void unload_handlers( struct _handler **list, int count ) {
	int n;


	for( n=0;n<count;n++ ) {
		free( list[n]->path );
		dlclose( list[n]->handle );
		free( list[n] );
		list[n] = NULL;
	}
}


int reload_handlers( char *base_dir, struct _handler ***list, int count ) {
	unload_handlers( *list, count );
	return load_handlers( base_dir, list, 0 );
}
