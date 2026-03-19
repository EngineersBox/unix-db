#include "parsing.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parseU8(char** buf, size_t* buf_len, uint8_t* result) {
    if (*buf_len < 1) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = (*buf)[0];
    *buf += 1;
    *buf_len -= 1;
    return 0;
}

int parseSizet(char** buf, size_t* buf_len, size_t* result) {
    if (*buf_len < sizeof(size_t)) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = atoi(*buf);
    *buf += sizeof(size_t);
    *buf_len -= sizeof(size_t);
    return 0;
}

int parseString(char**buf, size_t* buf_len, size_t str_len, char** result) {
    if (*buf_len < str_len) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = calloc(str_len, sizeof(char));
    if (*result == NULL) {
        return -ERROR_NO_MEMORY;
    }
    memcpy(*result, *buf, str_len * sizeof(char));
    *buf += str_len * sizeof(char);
    *buf_len -= str_len * sizeof(char);
    return 0;
}

int writeU8(char* buf, uint8_t value) {
    *buf = value;
    return 0;
}

int writeSizet(char* buf, size_t value) {
    snprintf(buf, sizeof(size_t), "%zu", value);
    return 0;
}

int writeString(char* buf, char* str, size_t str_len) {
    snprintf(buf, (str_len + 1) * sizeof(char), "%s", str);
    return 0;
}
