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

/* POSIX is part of the linux "real-time" library, so you need to
   compile with -lrt. */
int main (int argc, char **argv)
{
  /* int shm_open(const char *name, int oflag, mode_t mode);

     oflag is the open flag:
     RDWR = read/write
     CREAT = create segment if it doesn't already exist
     TRUNC = if it already exists, truncate it to 0 bytes

     mode:
     S_IRWXU = give Read Write and eXecute permission to the User

     type "man shm_open for explanation of o_flags
     type "man 2 open" for explanation of modes
  */
  int memory_handle = shm_open (segment_name, 
        O_RDWR | O_CREAT | O_TRUNC, 
        S_IRWXU);
  /* Check for errors */
  if (memory_handle == -1) {
    /* perror () prints its argument and then an explanation of the
       last system call / library call failure.*/
    perror("shm_open");
    exit (-1);
  }

  /* Set the size of the shared memory segment to be big enough to
     hold the SHARED_MEM struct we will use to communicate. */
  if (ftruncate (memory_handle, sizeof(SHARED_MEM)) == -1) {
    perror ("ftruncate");
    exit (-1);
  }

  /* Map shared memory segment into this process's address space.

     void *mmap(void *addr, size_t length, int prot, int flags, int
                  fd, off_t offset);
  */
  void *segment_addr = mmap (NULL,  /* have kernel assign address */
           sizeof (SHARED_MEM), /* size of area to map */
           PROT_WRITE | PROT_READ, /* Allow this
              process to
              read and write
              the memory */
           MAP_SHARED, /* allow other processes to
              see changes to this memory
              immediately */
           memory_handle, /* the shared memory
                 handle; this is the
                 area of kernel memory
                 to map into this
                 process's address
                 space */
           0); /* begin mapping at start (offset 0)
            of memory_handle */

  if (segment_addr == (void *) -1) {
    perror ("mmap");
    exit (-1);
  }

  /* At this point, we have a chunk of shared memory.  To access it,
     we have to superimpose some structure on it. */
  SHARED_MEM *chunk = (SHARED_MEM *) segment_addr;
  /* Now we can treat the shared memory like it is a struct of type
     SHARED_MEM.  Grab a string from the user, and store it in the
     shared memory. */

  printf ("Please enter some text (max %d chars): ", MAX_MESG_SIZE);

  /* Gets() reads an entire line of text from the standard input, up
     to and including the terminating '\n'.  It then replaces the
     newline with a '\0' character. 

     Read a string from the user and store it in the array in shared
     memory. */
  gets (chunk->mesg);
  chunk->mesg_size = strlen (chunk->mesg);

  /* No need to munmap the segment here.  The mapping will be
     destroyed when the process exits.

     We do not want to shm_unlink, because that will destroy the
     segment. */
  exit (0);
}