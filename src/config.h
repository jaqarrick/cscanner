#ifndef CONFIG_H
#include <stdbool.h>

#define CONFIG_H
#define NUM_THREADS 16
#define INIT_QUEUE_CAPACITY 256
typedef struct
{
    const char *ip;
    int start_port;
    int end_port;
    bool verbose;
} ScannerArgs;
void validate_args(int argc, char *argv[], ScannerArgs *scanner_args);
#endif // CONFIG_H