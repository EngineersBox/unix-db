#pragma once

#ifndef _UNIX_DB_QUERY_H_
#define _UNIX_DB_QUERY_H_

#include <stddef.h>

#define ERROR_INVALID_OPERATION 0x1

enum OperationType {
    OP_CREATE = 0,
    OP_DROP = 1,
    OP_SELECT = 2,
    OP_UPDATE = 3,
    OP_DELETE = 4,
};

#define OPERATION_TYPE_COUNT 5
extern const char* operation_type_tokens[OPERATION_TYPE_COUNT];

size_t operationParseType(char* buf, size_t buf_len, enum OperationType* out_type);

#endif // _UNIX_DB_QUERY_H_
