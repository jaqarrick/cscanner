#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>
#include "config.h"
#include "thread_pool.h"

extern ScannerArgs scanner_args;

int main(int argc, char *argv[])
{
    validate_args(argc, argv, &scanner_args);
    init_task_queue(INIT_QUEUE_CAPACITY);
    ThreadPool *pool = init_thread_pool(NUM_THREADS);
    init_jobs(pool);
    return 0;
}