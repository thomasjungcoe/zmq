// Reading from multiple sockets
// This version uses zmq_poll()

#include "../zhelpers.h"

int main (void) {
	// Connect to task ventilator
	void *context = zmq_ctx_new();
	void *receiver = zmq_socket(context, ZMQ_PULL);
	zmq_connect (receiver, "tcp://localhost:5557");

	// Connect to weather server
	void *subscriber = zmq_socket(context, ZMQ_SUB);
	zmq_connect (subscriber, "tcp://locahost:5556");
	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "10001", 6);

	// Process messages from both sockets
	while(1) {
		char msg[256];
		zmq_pollitem_t items [] = {
			{ receiver, 0, ZMQ_POLLIN, 0 },
			{ subscriber, 0, ZMQ_POLLIN, 0 }
		};

		// typedef struct {
		// 	 void *socket;	// 0MQ sockte to poll on
		//   int fd;		// OR, native file handle to poll on
		//	 short events;	// Events to poll on
		//	 short revents;	// Events returned after poll
		// } zmq_pollitem_t;

		zmq_poll (items, 2, -1);
		if (items[0].revents & ZMQ_POLLIN) {
			int size = zmq_recv(receiver, msg, 255, 0);
			if (size != -1) {
				// Process task
			}
		}
	}
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	return 0;
}