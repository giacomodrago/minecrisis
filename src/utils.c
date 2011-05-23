#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <float.h>
#include "utils.h"


#ifndef __WIN32__

#include <sys/time.h>

inline timestamp_t microtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long result = tv.tv_sec * 1000000;
    result += tv.tv_usec;
    return result;
}

#else

#define NOMINMAX 1
#include <windows.h>

/* http://www.decompile.com/cpp/faq/windows_timer_api.htm */

timestamp_t microtime() {

    LARGE_INTEGER ticksPerSecond;
    LARGE_INTEGER tick;   // A point in time

    // get the high resolution counter's accuracy
    QueryPerformanceFrequency(&ticksPerSecond);

    // what time is it?
    QueryPerformanceCounter(&tick);

    // convert the tick number into the number of seconds
    // since the system was started...
    seconds_t seconds = (seconds_t) tick.QuadPart / (seconds_t) ticksPerSecond.QuadPart;

    return (timestamp_t) (seconds * 1000000.0);

}

#endif

inline bool stripnl(char* string) {
    int pos = strlen(string)-1;
    if (string[pos] == '\n') {
        string[pos] = '\0';
        return true;
    }
    return false;
}

inline seconds_t msToSecs(int time) {
    return (seconds_t)time / 1000.0;
}

inline seconds_t usToSecs(timestamp_t time) {
    return (seconds_t)time / 1000000.0;
}

inline timestamp_t secsTous(seconds_t time) {
    return (timestamp_t) (time * 1000000.0);
}

inline double min(double a, double b) {
    return (a < b) ? a : b;
}

inline double max(double a, double b) {
    return (a > b) ? a : b;
}

double max_v(int count, ...) {
    va_list ap;
    double max_val = DBL_MIN;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        double val = va_arg(ap, double);
        if (val > max_val) max_val = val;
    }
    va_end(ap);
    return max_val;
}

double min_v(int count, ...) {
    va_list ap;
    double min_val = DBL_MAX;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        double val = va_arg(ap, double);
        if (val < min_val) min_val = val;
    }
    va_end(ap);
    return min_val;
}

int readFile(const char* filename, unsigned char** dest_buffer) {

    FILE* fp = fopen(filename, "rb");
    if (!fp) return FILE_READ_ERROR;

    int buffer_offset = 0;
    int buffer_allocated = FILE_READ_BLOCK_SIZE;
    unsigned char* buffer = (unsigned char*) malloc(buffer_allocated * sizeof(unsigned char));
    int read_bytes;

    while ((read_bytes = fread(buffer+buffer_offset, sizeof(unsigned char), FILE_READ_BLOCK_SIZE, fp)) > 0) {
        buffer_offset += read_bytes;
        if (buffer_offset + FILE_READ_BLOCK_SIZE > buffer_allocated) {
            buffer_allocated *= 2;
            buffer = (unsigned char*) realloc(buffer, buffer_allocated * sizeof(unsigned char));
        }
    }

    buffer = (unsigned char*) realloc(buffer, buffer_offset * sizeof(unsigned char));

    *dest_buffer = buffer;

    return buffer_offset;

}

bool checkCRC32(const char* filename, crc32_t expected_checksum) {
    unsigned char* buffer;
    int buf_len = readFile(filename, &buffer);
    if (buf_len == FILE_READ_ERROR) return false;
    crc32_t checksum = calculate_crc32(buffer, buf_len);
    free(buffer);
    if (checksum != expected_checksum) return false;
    return true;
}

