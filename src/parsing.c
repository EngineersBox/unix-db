#include "parsing.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parseU8(uint8_t** buf, size_t* buf_len, uint8_t* result) {
    if (*buf_len < 1) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = (*buf)[0];
    *buf += bytesof(char);
    *buf_len -= bytesof(char);
    return 0;
}

int parseSizet(uint8_t** buf, size_t* buf_len, size_t* result) {
    if (*buf_len < bytesof(size_t)) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = atoi((char*) *buf);
    *buf += bytesof(size_t);
    *buf_len -= bytesof(size_t);
    return 0;
}

int parseString(uint8_t**buf, size_t* buf_len, size_t str_len, char** result) {
    if (*buf_len < str_len) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = calloc(str_len, sizeof(char));
    if (*result == NULL) {
        return -ERROR_NO_MEMORY;
    }
    memcpy(*result, (char*) *buf, str_len * sizeof(char));
    *buf += str_len * sizeof(char);
    *buf_len -= str_len * sizeof(char);
    return 0;
}

int writeU8(uint8_t* buf, uint8_t value) {
    *buf = value;
    return 0;
}

int writeSizet(uint8_t* buf, size_t value) {
    snprintf((char*) buf, bytesof(size_t), "%zu", value);
    return 0;
}

int writeString(uint8_t* buf, char* str, size_t str_len) {
    snprintf((char*) buf, str_len, "%s", str);
    return 0;
}
