#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include <apr_general.h>

#include "libhandler.h"

#define SUPPORT_INIT( len ) char **list = ( char ** ) malloc ( sizeof( char * ) * len );int index=0
#define SUPPORT_ADD( name ) list[index++] = strdup( name )
#define SUPPORT_FINISH *outbound_list = list;return index;


int Handle( struct _handler_parameter *handle );

int supports( char ***list );
