#include "config.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ScannerArgs scanner_args;

void validate_port(int port)
{
    if (port < 1 || port > 65535)
    {
        printf("Invalid port number: %d. Port number must be between 1 and 65535.\n", port);
        exit(1);
    }
}

void validate_args(int argc, char *argv[], ScannerArgs *scanner_args)
{
    if (argc < 4)
    {
        printf("Usage: %s <IP> <start_port> <end_port>\n", argv[0]);
        exit(1);
    }

    scanner_args->ip = argv[1];
    scanner_args->start_port = atoi(argv[2]);
    scanner_args->end_port = atoi(argv[3]);
    scanner_args->verbose = false;

    validate_port(scanner_args->start_port);
    validate_port(scanner_args->end_port);

    if (scanner_args->end_port < scanner_args->start_port)
    {
        printf("Invalid port numbers. Starting port must be less than end port\n");
        exit(1);
    }
}
