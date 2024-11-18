#ifndef THREAD_POOL_H
#define THREAD_POOL_H
typedef struct Task
{
    int port;
} Task;
typedef struct TaskQueue
{
    int capacity;
    int size;
    Task *tasks;
} TaskQueue;
typedef struct ThreadPool {
    pthread_t *threads;
    int num_threads;
} ThreadPool;

void init_task_queue(int capacity);
void resize_task_queue(TaskQueue *queue);
void free_task_queue(TaskQueue *queue);
ThreadPool* init_thread_pool(int num_threads);
void init_jobs(ThreadPool *pool);
#endif // THREAD_POOL_H