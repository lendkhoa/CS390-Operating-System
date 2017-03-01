/* The name by which the shared memory segment will be known. It depends
to be unique on the system, so it should include your name.  In
addition, it can contain only alphanumeric chars and
underscores.  */
char *segment_name = "/le_shared_memory_segment"; 

/* The maximum message size we can store in the segment. */
#define MAX_MESG_SIZE 1024

/* The structure we will impose on the segment. */
typedef struct shared_mem {
  char mesg [MAX_MESG_SIZE];
  unsigned int mesg_size;
} SHARED_MEM;
