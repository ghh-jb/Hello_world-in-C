#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xpc/xpc.h>
#include <bootstrap.h> // for bootstrap_look_up

int main() {
	int p = 0;
	int our_pid = getpid();
	printf("[parent:success] Fork our pid: %i\n", our_pid);
	p = fork();

	if (p!= -1 && p != 0) {
		// Parent
		printf("[parent:success] p = %i => parent\n", p);
		// get parent process pid
		int fork_pid_parent = getpid();
		printf("[parent:success] parent pid: %i\n", fork_pid_parent);
		printf("[parent:success] Getting bootstrap_port...\n");

		mach_port_t bp_parent = 0;
		mach_port_t spParent = 0; // service port parent
		kern_return_t kr = task_get_bootstrap_port(mach_task_self_, &bp_parent);
		if (kr == KERN_SUCCESS) {
			if (!MACH_PORT_VALID(bp_parent)) {
				printf("[parent:fail] No valid bootstrap port %i\n", bp_parent);
				exit(-1);
			} else {
				printf("[parent:success] Got bootstrap port: %i\n", bp_parent);
				task_set_bootstrap_port(mach_task_self_, 0);
				printf("[parent:success] Set bootstrap port\n");
			}
		}
		
		kr = bootstrap_check_in(bp_parent, "fork_parent", &spParent);
		if (kr==KERN_SUCCESS) {
			printf("[parent:success] Registered service err: %i\n", kr);
			// return -1;
		} else {
			printf("[parent:fail] Failed to register service err: %i\n", kr);
			return -1;
		}

		struct {
			mach_msg_header_t header;
			char* text[64];
		} msg;

		printf("[parent:success] msg pre-struct generated, waiting for message...\n");

		// MACH_RECV_MSG - recieve message
		// MACH_MSG_TIMEOUT_NONE - no timeout
		// last is notice port??? MACH_PORT_NULL - not needed

		kr = mach_msg(&msg.header, MACH_RCV_MSG, 0, sizeof(msg), spParent, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL); // msg reciever 

		if (kr==KERN_SUCCESS) {
			printf("[parent:success] Message recieved:\n     msg:%s\n     err: %i\n", &msg.text, kr);
			printf("[parent:success] Exiting...");
			return 0;
		} else {
			printf("[parent:fail] Recieve message fail err: %s\n", mach_error_string(kr));
			return -1;
		}

	} else if (p == 0) {
		// Child
		printf("[child:success] p = %i => child\n", p);
		// get child process pid
		int fork_pid_child = getpid();
		printf("[child:success] child pid: %i\n", fork_pid_child);

		printf("[child:success] Getting bootstrap port...\n");
		// we DO NOT inherit bootstrap port from parent (child is a standalone process with its own bootstrap port)

		mach_port_t bp_child = 0;
		mach_port_t spChild = 0;

		kern_return_t kr = task_get_bootstrap_port(mach_task_self_, &bp_child); //get bootstrap port for child process
		if (kr==KERN_SUCCESS) {
			if (!MACH_PORT_VALID(bp_child)) {
				printf("[child:fail] No valid bootstrap port: %i\n", bp_child);
				exit(-1);
			} else {
				printf("[child:success] Got bootstrap port: %i\n", bp_child);
				task_set_bootstrap_port(mach_task_self_, 0);
				printf("[child:success] Set bootstrap port\n");
			}
		}

		kr = bootstrap_look_up(bp_child, "fork_parent", &spChild);

		if (kr==KERN_SUCCESS) {
			printf("[child:success] Service alive err: %i\n", kr);
		} else {
			printf("[child:fail] Service dead/stopped err: %i\n", kr);
		}

		struct {
			mach_msg_header_t header;
			char text[64];
		} msg;

		msg.header.msgh_bits = MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_COPY_SEND);
		msg.header.msgh_size = sizeof(msg);
		msg.header.msgh_remote_port = spChild; 
		msg.header.msgh_local_port = MACH_PORT_NULL;
		msg.header.msgh_id = 0;

		char userInput[256];

		printf("[child:input] Enter message: ");
		fgets(&userInput, 254, stdin);

		// strcpy(msg.text, "[child:msg:success] Hilo from child!"); //test

		userInput[strcspn(userInput, "\n\r")] = 0; // remove 1. Warging from gets 2. NEwlines
		strcpy(msg.text, &userInput);

		// MACH_SEND_MSG - send msg

		kr = mach_msg(&msg.header, MACH_SEND_MSG, sizeof(msg), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
		if (kr==KERN_SUCCESS) {
			printf("[child:success] Message to parent sent!\n");
			return 0;
		} else {
			printf("[child:fail] Message send failed => exiting...\n");
			return -1;
		}
	} else {
		// Fail
		printf("[parent:fail] p < 0 => fork error p = %i\n", p);
		return -1;
	}
}