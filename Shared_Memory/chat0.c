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
sem_t * sem_id;

/* Shared definitions */
#include "shm.h"

int main (int argc, char **argv)
{
  
  int memory_handle = shm_open (segment_name, O_RDWR, 0);
  if (memory_handle == -1) {
    perror ("shm_open");
    exit (-1);
  }

 /* Map shared memory segment into this process's address space. */
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

  printf ("Shared memory contained a string of %d characters:\n",
    chunk->mesg_size);
  printf ("\"%s\"\n", chunk->mesg);

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
