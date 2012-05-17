//
//  Hello World server
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

#include "libhandler.c"

int mainloop (char *url, char *base_dir)
{
	void *context = zmq_init (1);
	struct _handler **handler_list=NULL;
	int handler_count=0;


	apr_status_t rc;
	apr_pool_t *pool;


	rc = apr_initialize();
	rc = apr_pool_create(&pool, NULL);


	//  Socket to talk to clients
	void *responder = zmq_socket (context, ZMQ_REP);
	zmq_bind (responder, url);

	handler_count = load_handlers( base_dir, &handler_list, handler_count );
	while (1) {
		//  Wait for next request from client
		zmq_msg_t request;
		zmq_msg_init (&request);
		zmq_recv (responder, &request, 0);


		//load_handlers
		{
			int handled;
			int result;
			char *msg=zmq_msg_data( &request );
//			char *msg;
/*			
			{
				int len = zmq_msg_size( &request );
				char *buffer = apr_palloc( pool, len+1 );
				memcpy( buffer, zmq_msg_data( &request ), len );
				buffer[len] = '\0';
				syslog( LOG_DEBUG,  "Received: %s\n", buffer);
				
				msg = buffer;
			}
*/

			if ( !(strncmp( msg, "RELOAD HANDLERS", 15 ) ) ) {
				handler_count = reload_handlers( base_dir, &handler_list, handler_count );
			} else {
				int msg_size = zmq_msg_size( &request );
				result = bus_handle( pool, handler_list, handler_count, msg, msg_size, &handled );
			}
		}

		zmq_msg_close (&request);


		{
			zmq_msg_t reply;
			zmq_msg_init_size (&reply, 4);
			memcpy (zmq_msg_data (&reply), "done", 4);
			zmq_send (responder, &reply, 0);
			zmq_msg_close (&reply);
		}
	}
	//  We never get here but if we did, this would be how we end
	zmq_close (responder);
	zmq_term (context);
	return 0;
}
