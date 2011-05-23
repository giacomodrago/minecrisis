#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
typedef uint32_t crc32_t;

crc32_t calculate_crc32(const unsigned char *buf, unsigned long buflen);

#endif
