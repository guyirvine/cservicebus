#include "libhandler.h"


int _init_handler_parameter( apr_pool_t *pool, void *data, struct _handler_parameter **outbound_handler_parameter ) {
	int result=-1;
	struct _handler_parameter *parameter = NULL;

	parameter = apr_palloc( pool, sizeof( struct _handler_parameter ) );
	if ( parameter == NULL ) {
		return 0;
	}


	parameter->pool = pool;
	parameter->data = data;


	*outbound_handler_parameter = parameter;

	return -1;
}


int supported_by_handler( struct _handler *handler, char *discriminator ) {

	if ( handler != NULL ) {
		int i = handler->support_count;
		while( i-- ) {
			if ( !strcmp( handler->supports[i], discriminator ) ) {
				return -1;
			}
		}
	}
	return 0;

}


int handle_message( apr_pool_t *pool, void *handle, void *data ) {
	struct _handler_parameter *bus_handle = NULL;
	int (*Handle)( struct _handler_parameter *handle );

	char *error;
	int result;

	Handle = dlsym(handle, "Handle");
	if ((error = dlerror()) != NULL)  {
		syslog (LOG_ERR, "%s\n", error);
		return 0;
	}


	result = _init_handler_parameter( pool, data, &bus_handle );CHECK_FN_SUCCESS;
	result = (*Handle)( bus_handle );CHECK_FN_SUCCESS;


	return -1;
}

int bus_handle( apr_pool_t *pool, struct _handler **handler_list, int handler_count, char *msg, int msg_size, int *handled ) {
	int result ;
	int n = handler_count;
	char *discriminator;
	void *data=NULL;

	*handled = 0;
	
	discriminator = apr_palloc( pool, msg_size+1 );
	memcpy (discriminator, msg, msg_size);
	msg[msg_size] = '\0';

//    result = powder_xml2_parse( pool, buffer, &node );POWDER_CHECK_FN_SUCCESS;
//    discriminator = node->name;
//	discriminator = "SetAddressMessage";


	while( n-- ) {
		if ( supported_by_handler( handler_list[n], discriminator ) ) {
			syslog( LOG_ERR,  "Handling: %s->%s\n", discriminator, handler_list[n]->path );
			result = handle_message( pool, handler_list[n]->handle, data );CHECK_FN_SUCCESS;
			*handled = -1;
			break;
		}
	}


	if ( *handled == 0 ) {
		syslog( LOG_ERR,  "Not Handling: %s\n", discriminator );
	}


	return -1;
}
