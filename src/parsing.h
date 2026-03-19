#pragma once

#ifndef _UNIX_DB_PARSING_H_
#define _UNIX_DB_PARSING_H_

#include <stddef.h>
#include <stdint.h>

#define ERROR_BUF_TOO_SMALL 0x1
#define ERROR_NO_MEMORY 0x2

int parseU8(char** buf, size_t* buf_len, uint8_t* result);
int parseSizet(char** buf, size_t* buf_len, size_t* result);
int parseString(char**buf, size_t* buf_len, size_t str_len, char** result);

int writeU8(char* buf, uint8_t value);
int writeSizet(char* buf, size_t value);
int writeString(char* buf, char* str, size_t str_len);

#endif // _UNIX_DB_PARSING_H_
