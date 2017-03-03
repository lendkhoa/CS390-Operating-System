/* For shm* functions */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h> 
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

/* To get perror() */
#include <errno.h>

/* Shared definitions */
#include "shm.h"

/* POSIX is part of the linux "real-time" library, so you need to
   compile with -lrt. */
int main (int argc, char **argv)
{
  
  int memory_handle = shm_open (segment_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
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

  if (shm_unlink (segment_name) == -1) {
    perror ("shm_unlink");
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
