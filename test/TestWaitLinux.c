#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ae2f/Lock.h>

#define THREAD_COUNT 2
ae2f_addrel_t sharedFlag = 0; // Shared variable for synchronization

// Producer thread: Sets the flag and wakes waiting threads
void* ProducerThread(void* param) {
    int threadId = *(int*)param;
    printf("Producer %d: Starting\n", threadId);
    
    // Simulate some work
    sleep(2);
    
    printf("Producer %d: Setting flag to 1\n", threadId);
    sharedFlag = 1;
    __ae2f_WakeSingle(&sharedFlag); // Wake one waiting thread
    
    printf("Producer %d: Finished\n", threadId);
    return NULL;
}

// Consumer thread: Waits for the flag to change
void* ConsumerThread(void* param) {
    int threadId = *(int*)param;
    printf("Consumer %d: Starting, waiting for flag\n", threadId);
    
    // Small delay to ensure producer hasn't already set the flag
    usleep(100000); // 100ms
    
    printf("Consumer %d: sharedFlag value before wait: %d\n", threadId, sharedFlag);
    
    // Wait until sharedFlag becomes 1
    int rc = __ae2f_Wait(&sharedFlag, 0);
    if (rc != 0) {
        printf("Consumer %d: Futex wait failed, errno: %d\n", threadId, errno);
    }
    
    printf("Consumer %d: Flag is now %d, proceeding\n", threadId, sharedFlag);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int threadParams[THREAD_COUNT];
    
    // Create one producer and one consumer thread
    for (int i = 0; i < THREAD_COUNT; i++) {
        threadParams[i] = i + 1;
        int rc = pthread_create(&threads[i], NULL, 
                               i == 0 ? ProducerThread : ConsumerThread, 
                               &threadParams[i]);
        if (rc != 0) {
            printf("Failed to create thread %d\n", i + 1);
            return 1;
        }
    }
    
    // Wait for both threads to complete
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads completed\n");
    return 0;
}