// ROUTER-to-DEALER example

#include "../zhelpers.h"
#include <pthread.h>
#define NBR_WORKERS 10

static void *
worker_task(void *args) {
	void *context = zmq_ctx_new();
	void *worker = zmq_socket(context, ZMQ_DEALER);
	s_set_id(worker);		// Set a printable identity
	zmq_connect (worker, "tcp://localhost:5671");

	int total = 0;
	while(1) {
		// Tell the broker we're ready for work
		s_sendmore(worker, "");
		s_send(worker, "Hi Boss");

		// Get workload from broker, until finished
		free (s_recv(worker));		// Envelope delimiter
		char *worload = s_recv(worker);
	}
}

