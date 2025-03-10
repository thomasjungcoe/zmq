// SImple request-reply broker

#include "../zhelpers.h"

int main(void) {

	// Prepare our context and sockets
	void *context  = zmq_ctx_new();
	void *frontend = zmq_socket(context, ZMQ_ROUTER);
	void *backend  = zmq_socket(context, ZMQ_DEALER);
	zmq_bind (frontend, "tcp://*:5559");
	zmq_bind (backend , "tcp://*:5560");

	// Initialze poll set
	zmq_pollitem_t items [] = {
		{ frontend, 0, ZMQ_POLLIN, 0 },
		{ backend , 0, ZMQ_POLLIN, 0 }
	};

	// switch msgs between sockets
	while(1) {
		zmq_msg_t message;
		zmq_poll (items, 2, -1);
		if (items[0].revents & ZMQ_POLLIN) {
			while(1) {
				// Process all parts of message
				zmq_msg_init (&message);
				zmq_msg_recv (&message, frontend, 0);
				zmq_msg_send (&message, backend, 
					zmq_msg_more (&message)? ZMQ_SNDMORE : 0);
				zmq_msg_close(&message);
				if (!zmq_msg_more (&message)) break; // Last message part				
			}
		}
		if (items [1].revents & ZMQ_POLLIN) {
			while(1) {
				// Process all parts of the message
				zmq_msg_init(&message);
				zmq_msg_recv(&message, backend, 0);
				zmq_msg_send(&message, frontend, 
					zmq_msg_more (&message) ? ZMQ_SNDMORE : 0);
				zmq_msg_close(&message);
				if (!zmq_msg_more (&message)) break;	// Last msg part
			}
		}
	}
	// We never get here, but clean up anyhow
	zmq_close(frontend);
	zmq_close(backend);
	zmq_ctx_destroy(context);

	return 0;

}