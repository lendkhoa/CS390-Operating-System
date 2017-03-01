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

/* POSIX is part of the linux "real-time" library, so you need to
   compile with -lrt. */
int main (int argc, char **argv)
{

  signal(SIGINT, signal_callback_handler);
  
  int memory_handle = shm_open (segment_name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
  /* Check for errors */
  if (memory_handle == -1) {
    perror("shm_open");
    exit (-1);
  }

 if (ftruncate (memory_handle, sizeof(SHARED_MEM))) {
    perror ("ftruncate");
    exit (-1);
  }

  void *segment_addr = mmap (NULL, sizeof (SHARED_MEM), PROT_WRITE | PROT_READ, MAP_SHARED, memory_handle, 0);

  if (segment_addr == (void *) -1) {
    perror ("mmap");
    exit (-1);
  }

  SHARED_MEM *chunk = (SHARED_MEM *) segment_addr;

  while(1) {
	printf ("Please enter some text (max %d chars): ", MAX_MESG_SIZE);
	gets (chunk->mesg);
	if (!strcmp(chunk->mesg, "END")) exit(0);

	chunk->mesg_size = strlen (chunk->mesg);
  }

  exit (0);
}