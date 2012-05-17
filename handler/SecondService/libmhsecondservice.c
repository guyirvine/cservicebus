#include "libmhsecondservice.h"

int Handle( struct _handler_parameter *parameter ) {
    int result = -1;

	syslog( LOG_DEBUG, "SecondService.0" );
	syslog( LOG_DEBUG, "SecondService.2" );
/*
    if ( !strcmp( "SetAddressMessage", bus_handle->node->name ) ) {
      syslog( LOG_DEBUG, "AddressService.1" );
        result = process_setaddress( bus_handle->pool, bus_handle->conn, bus_handle->node, bus_handle->msg_send_list );POWDER_CHECK_FN_SUCCESS;
      syslog( LOG_DEBUG, "AddressService.2" );
    } else if ( !strcmp( "DeleteAddressMessage", bus_handle->node->name ) ) {
        result = process_deleteaddress( bus_handle->pool, bus_handle->conn, bus_handle->node, bus_handle->msg_send_list );POWDER_CHECK_FN_SUCCESS;
    }
*/

    return 1;
}

int supports( char ***outbound_list ) {
    SUPPORT_INIT( 2 );

    SUPPORT_ADD( "SetAddressMessage" );
    SUPPORT_ADD( "DeleteAddressMessage" );

    SUPPORT_FINISH;
}
