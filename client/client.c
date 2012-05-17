//
//  Hello World client
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main2 (int argv, char **argc)
{
	char *req_buf;
//	char *reply_buf;
	char reply_buf[2048];

	void *context = zmq_init (1);

	//  Socket to talk to server
	void *requester = zmq_socket (context, ZMQ_REQ);
	zmq_connect (requester, "tcp://localhost:5555");
	

	zmq_msg_t request;
	
	req_buf = argc[1];
	int buffer_size = strlen( req_buf );

	zmq_msg_init_size (&request, buffer_size);
	memcpy (zmq_msg_data (&request), req_buf, buffer_size);
//	printf ("Sending: %s\n", req_buf);
	zmq_send (requester, &request, 0);
	zmq_msg_close (&request);

	zmq_msg_t reply;
	zmq_msg_init (&reply);
	zmq_recv (requester, &reply, 0);
	{
		int buf_size;
		buf_size = zmq_msg_size( &reply );
		memcpy (reply_buf, zmq_msg_data( &reply ), buf_size);
		reply_buf[buf_size] = '\0';
	}
	zmq_msg_close (&reply);

	printf ("about to log\n" );
	printf ("Reply:%s-%s\n", req_buf, reply_buf);

	zmq_close (requester);
	zmq_term (context);
	return 0;
}

int main (int argv, char **argc)
{
	int i;
	for( i=0;i<1;i++ ) {
		main2 (argv, argc);
	}
}
