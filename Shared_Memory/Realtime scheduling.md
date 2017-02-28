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



