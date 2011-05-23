#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "crc32.h"

#define FILE_READ_BLOCK_SIZE 131072
#define FILE_READ_ERROR -1

typedef unsigned long long timestamp_t;
typedef long double seconds_t;

typedef struct {
    unsigned char* data;
    int data_len;
    int data_pos;
} data_stream_t;

timestamp_t microtime();
bool stripnl(char* string);
seconds_t msToSecs(int time);
seconds_t usToSecs(timestamp_t time);
timestamp_t secsTous(seconds_t time);
double min(double a, double b);
double max(double a, double b);
double max_v(int count, ...);
double min_v(int count, ...);
int readFile(const char* filename, unsigned char** dest_buffer);
bool checkCRC32(const char* filename, crc32_t expected_checksum);


#endif
