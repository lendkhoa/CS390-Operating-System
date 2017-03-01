#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "shm.h"

int main (int argc, char **argv)
{
  /* Get a handle to the existing shared memory segment. Note that
     this will fail if the segment doesn't exist. */
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

  /* Again, we will do this in two steps:
     1) Copy data from shared memory to process-local memory.
     2) Print the data. */
  SHARED_MEM *chunk = (SHARED_MEM *) segment_addr;

  printf ("Shared memory contained a string of %d characters:\n",
    chunk->mesg_size);
  printf ("\"%s\"\n", chunk->mesg);

  /* Finally, unmap the shared memory segment from our address
     space. */
  if (munmap (segment_addr, sizeof (SHARED_MEM)) == -1) {
    perror ("munmap");
    exit (-1);
  }

  /* Free the shared memory segment. Shm_unlink() just schedules the
     deletion of the segment. It will actually be deleted only after
     the last process that has mmap()'d to it munmap()'s it. */
  if (shm_unlink (segment_name) == -1) {
    perror ("shm_unlink");
    exit (-1);
  }

  exit(0);
}
  