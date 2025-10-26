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
    // Make shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(shared_memory));
    shared_memory *shm = mmap(NULL, sizeof(shared_memory),
                              PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // initalize the buffer
    for(int i = 0; i < BUFFER_SIZE; i++) shm->buffer[i] = -1;
    shm->in = 0;
    shm->out = 0;

    // Create semaphores
    sem_unlink("/mutex"); sem_unlink("/empty"); sem_unlink("/full"); // clean up previous runs
    sem_t *mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t *empty = sem_open("/empty", O_CREAT, 0666, BUFFER_SIZE);
    sem_t *full  = sem_open("/full",  O_CREAT, 0666, 0);

    int item = 1;
    while(item <= 10) {
        sem_wait(empty);
        sem_wait(mutex);

        shm->buffer[shm->in] = item;
        printf("Produced %d ", item);
        print_buffer(shm);

        shm->in = (shm->in + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(full);

        item++;
        sleep(1);
    }

    sem_close(mutex); sem_close(empty); sem_close(full);
    return 0;
}
