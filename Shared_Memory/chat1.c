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
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

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
	int memory_handle;
    int vol, cur;
    int shared_seg_size = (1 * sizeof(SHARED_MEM));   /* want shared segment capable of storing 1 message */
    sem_t *sema; /* pointer to sem */
    int val;

    signal(SIGINT, signal_callback_handler);

    /* creating the shared memory object    --  shm_open()  */
    memory_handle = shm_open(segment_name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (memory_handle < 0)
    {
        perror("In shm_open()");
        exit(1);
    }

    ftruncate(memory_handle, shared_seg_size);

    /* requesting the shared segment    --  mmap() */
    void *segment_addr = mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, memory_handle, 0);
    if (segment_addr == (void *) -1)
    {
        perror("In mmap()");
        exit(1);
    }

    /*superimpose the structure */
    SHARED_MEM *chunk = (SHARED_MEM *) segment_addr;
    
    while(1)
    {
    	printf ("Shared memory contained a string of %d characters:\n",
		chunk->mesg_size);
		printf("chat0 : %s\n", chunk->mesg);

    	printf ("Please enter some text to send to 0 (max %d chars): ", MAX_MESG_SIZE);
    	gets (chunk->mesg);
  		chunk->mesg_size = strlen (chunk->mesg);
  		if (!strcmp(chunk->mesg, "END")) { exit(-1); }
    }

    if (shm_unlink(segment_name) != 0) {
        perror("In shm_unlink()");
        exit(1);
    }

    return 0;
}