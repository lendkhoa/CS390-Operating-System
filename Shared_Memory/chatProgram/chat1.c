/*
 * Project 1 shared memory and semaphore 
 * Khoa Le, Truc Ha
 * Dr. Matthews
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

#include "shm.h"
int main (int argc, char **argv)
{
  int memory_handle = shm_open (segment_name, O_RDWR, 0);
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
  
  while(1) {
    sem_wait(&chunk->sema1);
    
    printf ("Chat0 says: %s\n", chunk->mesg);
    if (strcmp(chunk->mesg, "END\n") == 0) {
     break;
    }

    printf ("Please enter some text to send to 0 (max %d chars): ", MAX_MESG_SIZE);
    
    fgets(chunk->mesg, 1024, stdin);
    chunk->mesg_size = strlen (chunk->mesg);
    if (strcmp(chunk->mesg, "END\n") == 0) {
     sem_post(&chunk->sema); 
     break;
    }
   
    sem_post(&chunk->sema); 
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
  
