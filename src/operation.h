#pragma once

#ifndef _UNIX_DB_QUERY_H_
#define _UNIX_DB_QUERY_H_

#include <stddef.h>

#define ERROR_INVALID_OPERATION 0x1
#define ERROR_INVALID_OP_CREATE 0x2
#define ERROR_INVALID_OP_DROP 0x3

enum OperationType {
    OP_CREATE = 0,
    OP_DROP = 1,
    OP_SELECT = 2,
    OP_UPDATE = 3,
    OP_DELETE = 4,
};

#define OPERATION_TYPE_COUNT 5
extern const char* operation_type_tokens[OPERATION_TYPE_COUNT];

size_t operationTypeParse(char* buf, size_t buf_len, enum OperationType* out_type);

enum OpCreateType {
    OP_CREATE_TABLE = 0,
};

#define OP_CREATE_TYPE_COUNT 1
extern const char* op_create_type_tokens[OP_CREATE_TYPE_COUNT];

size_t opCreateTypeParse(char* buf, size_t buf_len, enum OpCreateType* out_type);

enum OpDropType {
    OP_DROP_TABLE = 0,
};

#define OP_DROP_TYPE_COUNT 1
extern const char* op_drop_type_tokens[OP_DROP_TYPE_COUNT];

size_t opDropTypeParse(char* buf, size_t buf_len, enum OpDropType* out_type);

#endif // _UNIX_DB_QUERY_H_
