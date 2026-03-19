#pragma once

#ifndef _UNIX_DB_PARSING_H_
#define _UNIX_DB_PARSING_H_

#include <stddef.h>
#include <stdint.h>

#define ERROR_BUF_TOO_SMALL 0x1
#define ERROR_NO_MEMORY 0x2

#define bytesof(t) (sizeof(t) / sizeof(uint8_t))

int parseU8(uint8_t** buf, size_t* buf_len, uint8_t* result);
int parseSizet(uint8_t** buf, size_t* buf_len, size_t* result);
int parseString(uint8_t**buf, size_t* buf_len, size_t str_len, char** result);

int writeU8(uint8_t* buf, uint8_t value);
int writeSizet(uint8_t* buf, size_t value);
int writeString(uint8_t* buf, char* str, size_t str_len);

#endif // _UNIX_DB_PARSING_H_
