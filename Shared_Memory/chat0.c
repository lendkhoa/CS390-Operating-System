#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include <shm.h>

int main(int argc, char **argv) {

	int fd = shm_open (const char * shard_name, int oflag, int mode);
	if (fd == -1) {
		perror('shm_open');
		exit(-1);
	}

	int size_shm = ftruncate(int fd, unsigned int length);
	if (size_shm == -1) {
		perror('ftruncate');
		exit(-1);
	}

	void *segment_addr = mmap (void *addr, unsigned int length, int protection, int flags, int fd, unsigned int offset);
	if (segment_addr == (void *) -1) {
		perror("mmap");
		exit(-1);
	}

	

}
