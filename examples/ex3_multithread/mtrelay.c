// Multithreaded relay

#include "zhelpers.h"
#include <pthreads.h>

static void *
step1 (void *context) {
	// Connect to step2 and tell it we're aredy
	void *xmitter = zmq_socket(context, ZMQ_PAIR);
	zmq_connect (xmitter, "inproc://step2");
	printf("Step 1 ready, signalling step 2\n");
	s_send(xmitter, "READY");
	zmq_close(xmitter);

	return NULL;
}

static void*
step2 (void *context) {
	// Bind inproc socket before starting step1
	void *receiver = zmq_socket(context, ZMQ_PAIR);
	zmq_bind(receiver, "inproc://step2")
	pthread_t thread;
	pthread_create(&thread, NULL, step1, context);

	
}