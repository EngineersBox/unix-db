#include "parsing.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t parseU8(char* buf, size_t buf_len, uint8_t* result) {
    if (buf_len < 1) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = buf[0];
    return 1;
}

size_t parseSizet(char* buf, size_t buf_len, size_t* result) {
    if (buf_len < sizeof(size_t)) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = atoi(buf);
    return sizeof(size_t);
}

size_t parseString(char* buf, size_t buf_len, size_t str_len, char** result) {
    if (buf_len < str_len) {
        return -ERROR_BUF_TOO_SMALL;
    }
    *result = calloc(str_len, sizeof(char));
    if (*result == NULL) {
        return -ERROR_NO_MEMORY;
    }
    memcpy(*result, buf, str_len * sizeof(char));
    return str_len * sizeof(char);
}

size_t parseStringUntil(char* buf, size_t buf_len, char until, char** result) {
    if (buf_len <= 0) {
        return -ERROR_BUF_TOO_SMALL;
    }
    size_t str_len = 0;
    char* end = buf;
    while (*end != until) {
        if (str_len >= buf_len) {
            return -ERROR_BUF_TOO_SMALL;
        }
        str_len++;
        end++;
    }
    *result = calloc(str_len, sizeof(char));
    if (*result == NULL) {
        return -ERROR_NO_MEMORY;
    }
    memcpy(*result, buf, str_len * sizeof(char));
    return str_len * sizeof(char);
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
