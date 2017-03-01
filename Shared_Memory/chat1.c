/* For shm* functions */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/* To get strnlen(), memcpy(), etc. */
#include <string.h> 

#include <stdio.h>
#include <stdlib.h>

/* To get perror() */
#include <errno.h>

/* Shared definitions */
#include "shm.h"

void signal_callback_handler(int signum)
{

        /**
         * Semaphore unlink: Remove a named semaphore  from the system.
         */
        if ( shm_unlink("/mysem") < 0 )
        {
                perror("shm_unlink");
        }

   // Terminate program
   exit(signum);
}

int main (int argc, char *argv[]) {
	signal(SIGINT, signal_callback_handler);
	int memory_handle = shm_open (segment_name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
	if (memory_handle == -1) {
		perror ("shm_open");
		exit (-1);
	}

	void *segment_addr = mmap (NULL, 
			     sizeof (SHARED_MEM),
			     PROT_READ | PROT_WRITE,
			     MAP_SHARED, 
			     memory_handle,
			     0);
	if (segment_addr == (void *) -1) {
	perror ("mmap");
	exit (-1);
	}

	SHARED_MEM *chunk = (SHARED_MEM *) segment_addr;

	while(1){
		printf ("Shared memory contained a string of %d characters:\n",
		chunk->mesg_size);
		printf ("\"%s\"\n", chunk->mesg);
		sleep(3);
	}

	if (munmap (segment_addr, sizeof (SHARED_MEM)) == -1) {
		perror ("munmap");
		exit (-1);
	}

	if (shm_unlink (segment_name) == -1) {
		perror ("shm_unlink");
		exit (-1);
	}

	exit(0);
}