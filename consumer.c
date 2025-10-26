#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>

#define SHM_NAME "/my_shm"
#define BUFFER_SIZE 2

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_memory;

void print_buffer(shared_memory *shm) {
    printf("[");
    for(int i = 0; i < BUFFER_SIZE; i++) {
        if(shm->buffer[i] == -1) printf("__");
        else printf("%d", shm->buffer[i]);
        if(i < BUFFER_SIZE-1) printf(",");
    }
    printf("]\n");
}

int main() {
    int fd;
    shared_memory *shm;

    // Wait for producer for shared memory
    while(1) {
        fd = shm_open(SHM_NAME, O_RDWR, 0666);
        if(fd != -1) break;
        usleep(100000); // 0.1 sec
    }
    shm = mmap(NULL, sizeof(shared_memory),
               PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Wait for semaphores
    sem_t *mutex, *empty, *full;
    while(1) {
        mutex = sem_open("/mutex", 0);
        empty = sem_open("/empty", 0);
        full  = sem_open("/full", 0);
        if(mutex != SEM_FAILED && empty != SEM_FAILED && full != SEM_FAILED) break;
        usleep(100000);
    }

    int item;
    for(int i = 0; i < 10; i++) {
        sem_wait(full);
        sem_wait(mutex);

        item = shm->buffer[shm->out];
        shm->buffer[shm->out] = -1;
        printf("Consumed %d ", item);
        print_buffer(shm);

        shm->out = (shm->out + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(empty);

        sleep(2);
    }

    sem_close(mutex); sem_close(empty); sem_close(full);
    sem_unlink("/mutex"); sem_unlink("/empty"); sem_unlink("/full");
    shm_unlink(SHM_NAME);

    return 0;
}
