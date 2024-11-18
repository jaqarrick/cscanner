#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "thread_pool.h"
#include "scanner.h"
pthread_mutex_t mutex_queue;
pthread_cond_t cond_queue;
extern ScannerArgs scanner_args;
int terminate = 0;

TaskQueue taskQueue;
void init_task_queue(int capacity)
{
    if(scanner_args.verbose){
        printf("initiating task queue with capacity of %d\n", capacity);
    }
    taskQueue.tasks = (Task *)malloc(capacity * sizeof(Task));
    taskQueue.size = 0;
    taskQueue.capacity = capacity;
}

void resize_task_queue(TaskQueue *queue)
{
    queue->capacity *= 2;
    queue->tasks = (Task *)realloc(queue->tasks, queue->capacity * sizeof(Task));
}

void free_task_queue(TaskQueue *queue)
{
    free(queue->tasks);
}

void submit_task(Task task)
{
    pthread_mutex_lock(&mutex_queue);

    if (taskQueue.size == taskQueue.capacity)
    {
        resize_task_queue(&taskQueue);
    }
    taskQueue.tasks[taskQueue.size++] = task;
    pthread_mutex_unlock(&mutex_queue);
    pthread_cond_signal(&cond_queue);
}


void execute_task(Task *task)
{
    int connection = scan_port(task->port);
    if (connection == 1)
    {
        printf("warn: error creating socket\n");
        exit(1);
    }

    if (connection == 0)
    {
        printf("%s:%d/TCP: connection success\n", scanner_args.ip, task->port);
    }
}

void *start_thread(void *args)
{
    while (1)
    {
        Task task;
        pthread_mutex_lock(&mutex_queue);

        while (taskQueue.size == 0 && !terminate)
        {
            pthread_cond_wait(&cond_queue, &mutex_queue);
        }
        if (terminate && taskQueue.size == 0)
        {
            pthread_mutex_unlock(&mutex_queue);
            break;
        }

        task = taskQueue.tasks[0];
        
        memmove(&taskQueue.tasks[0], &taskQueue.tasks[1], (taskQueue.size - 1) * sizeof(Task));
        taskQueue.size--;

        pthread_mutex_unlock(&mutex_queue);
        execute_task(&task);
    }
    return NULL;
}

ThreadPool *init_thread_pool(int num_threads)
{
    bool v = scanner_args.verbose;
    // create the thread pool

    if(v){
        printf("initiating mutex and cond variable\n");
    }
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_cond_init(&cond_queue, NULL);

    if(v){
        printf("initiating thread pool with %d threads\n", num_threads);
    }
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    if (!pool)
    {
        perror("Failed to create thread pool\n");
    }
    pool->num_threads = num_threads;
    pool->threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

    if (!pool->threads)
    {
        free(pool);
        perror("Failed to create thread pool\n");
    }

    int i = 0;
    for (i = 0; i < pool->num_threads; i++)
    {
        if (pthread_create(&pool->threads[i], NULL, &start_thread, NULL) != 0)
        {
            perror("Failed to create thread ");
        }
    }

    return pool;
}

void init_jobs(ThreadPool *pool)
{

    for (int i = scanner_args.start_port; i <= scanner_args.end_port; i++)
    {
        Task t = {
            .port = i};
        submit_task(t);
    }

    pthread_mutex_lock(&mutex_queue);
    terminate = 1;
    pthread_mutex_unlock(&mutex_queue);
    pthread_cond_broadcast(&cond_queue);
    for (int i = 0; i < pool->num_threads; i++)
    {
        if (pthread_join(pool->threads[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutex_queue);
    pthread_cond_destroy(&cond_queue);
    free_task_queue(&taskQueue);
}
