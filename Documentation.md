## Description

This program demonstrates the Producer Consumer problem. This program was written in C. 

It uses shared memory to allow both the producer and consumer to use the buffer. POSIX semaphores are used to synchronize access of the buffer. 

The producer generates a numbered item and adds to buffer. This is displayed in a table output. Mutual exclusion is used for proper synchronization.

The consumer removes the item, and this is also displayed in the output. The producer and consumer communicate through shared memory. 

## Components

### Shared Memory
Implemented using shm_open, ftruncate, and mmap. 

Provides place for communication between producer and consumer.

### Semaphores
/mutex used for mutual exclusion.

/empty keeps track of how many empty slots. Sees if producer needs to wait.

/full keeps track of how many full slots. Sees if consumer can consume or not.


### Communication
How the producer communicates: Writes items into memory.

How the consumer communicates: Reads items.

Semaphores prevent race conditions.
