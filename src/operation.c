#include "operation.h"

#include "parsing.h"
#include <string.h>

const char* operation_type_tokens[OPERATION_TYPE_COUNT] = {
    "CREATE",
    "DROP",
    "SELECT",
    "UPDATE",
    "DELETE"
};

const char* op_create_type_tokens[OP_CREATE_TYPE_COUNT] = {
    "TABLE",
};

const char* op_drop_type_tokens[OP_DROP_TYPE_COUNT] = {
    "TABLE"
};

size_t operationTypeParse(char* buf, size_t buf_len, enum OperationType* out_type) {
    char* str = NULL;
    size_t str_len = parseStringUntil(buf, buf_len, ' ', &str);
    if (str_len <= 0) {
        return str_len;
    }
    enum OperationType type = -1;
    for (int i = 0; i < OPERATION_TYPE_COUNT; i++) {
        if (strncmp(str, operation_type_tokens[i], str_len) == 0) {
            type = (enum OperationType) i;
            break;
        }
    }
    if (type == -1) {
        return -ERROR_INVALID_OPERATION;
    }
    *out_type = type;
    return str_len;
}

size_t opCreateTypeParse(char* buf, size_t buf_len, enum OpCreateType* out_type) {
    char* str = NULL;
    size_t str_len = parseStringUntil(buf, buf_len, ' ', &str);
    if (str_len <= 0) {
        return str_len;
    }
    enum OpCreateType type = -1;
    for (int i = 0; i < OP_CREATE_TYPE_COUNT; i++) {
        if (strncmp(str, op_create_type_tokens[i], str_len) == 0) {
            type = (enum OpCreateType) i;
            break;
        }
    }
    if (type == -1) {
        return -ERROR_INVALID_OP_CREATE;
    }
    *out_type = type;
    return str_len;
}

size_t opDropTypeParse(char* buf, size_t buf_len, enum OpDropType* out_type) {
    char* str = NULL;
    size_t str_len = parseStringUntil(buf, buf_len, ' ', &str);
    if (str_len <= 0) {
        return str_len;
    }
    enum OpDropType type = -1;
    for (int i = 0; i < OP_DROP_TYPE_COUNT; i++) {
        if (strncmp(str, op_drop_type_tokens[i], str_len) == 0) {
            type = (enum OpDropType) i;
            break;
        }
    }
    if (type == -1) {
        return -ERROR_INVALID_OP_DROP;
    }
    *out_type = type;
    return str_len;
}
