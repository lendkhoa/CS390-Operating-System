/*
 * Project 1 shared memory and semaphore 
 * Khoa Le, Truc Ha
 * Dr. Matthews
 */

/* For shm* functions */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

/* To get strnlen(), memcpy(), etc. */
#include <string.h> 

#include <stdio.h>
#include <stdlib.h>

/* To get perror() */
#include <errno.h>

/* Shared definitions */
#include "shm.h"

/* semaphore library */
#include <semaphore.h>

int main (int argc, char **argv)
{
   int memory_handle = shm_open (segment_name, 
          O_RDWR | O_CREAT | O_TRUNC, 
          S_IRWXU);
   /* Check for errors */
   if (memory_handle == -1) {
     perror("shm_open");
     exit (-1);
   }

   if (ftruncate (memory_handle, sizeof(SHARED_MEM)) == -1) {
     perror ("ftruncate");
     exit (-1);
   }

   void *segment_addr = mmap (NULL,
                              sizeof(SHARED_MEM), 
                              PROT_WRITE | PROT_READ, 
                              MAP_SHARED, 
                              memory_handle, 
                              0);

   if (segment_addr == (void *) -1) {
      perror ("mmap");
      exit (-1);
   }

   SHARED_MEM *chunk = (SHARED_MEM *) segment_addr;
   sem_init(&chunk->sema, 1, 1);
   sem_init(&chunk->sema1, 1, 0);
   
   while (1) {
    sem_wait(&chunk->sema);
    
    if((chunk->mesg_size) != 0) {
     printf("Chat1 says: %s\n", chunk->mesg);
    }
    if (strcmp("END\n", chunk->mesg) == 0) {
     break;
    }
    printf ("Please enter some text to send to 1 (max %d chars): ", MAX_MESG_SIZE);
    fgets (chunk->mesg, 1024, stdin);
    
    chunk->mesg_size = strlen (chunk->mesg);
    if (strcmp("END\n", chunk->mesg) == 0) {
     sem_post(&chunk->sema1);
     break;
    }
    
    sem_post(&chunk->sema1);  
   }
   
   if (munmap (segment_addr, sizeof (SHARED_MEM)) == -1) {
      perror ("munmap");
      exit (-1);
    }

   exit (0);
}
