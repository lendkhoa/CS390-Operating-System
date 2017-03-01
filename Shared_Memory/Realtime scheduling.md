# Realtime scheduling
[source] (http://www.gnu.org/software/libc/manual/html_node/Realtime-Scheduling.html)


Whenever two processes with the same absolute priority are ready to run, 
the kernel has a decision to make, because only one can run at a time

Each process has a scheduling policy. For processes with absolute 
priority other than zero, there are two available:

1. First Come First Served
2. Round Robin


The only difference between a process that is assigned the Round Robin 
scheduling policy and a process that is assigned First Come First Serve 
is that in the former case, the process is automatically booted off the 
CPU after a certain amount of time

### Shared Memory
 When a new shared memory object is created it is given the owner and group corresponding to the effec-
tive user and group of the calling process. There is no visible entry in the file system for the cre-
ated object in this implementation.

When a shared memory object is created, it persists until it it unlinked and all other references are
gone. Objects do not persist across a system reboot.

