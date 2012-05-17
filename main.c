#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK_FN_SUCCESS if( result == 0 ) { return 0; }

#include "zeromq.c"


int main(void) {

	char *base_dir = "/usr/local/cservicebus/lib/message_handler/";
	mainloop( "tcp://*:5555", base_dir );

	return 0;
}
