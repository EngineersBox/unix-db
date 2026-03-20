#pragma once

#ifndef _UNIX_DB_QUERY_H_
#define _UNIX_DB_QUERY_H_

#include <stddef.h>

enum OpType {
    OP_CREATE = 0,
    OP_DROP = 1,
    OP_SELECT = 2,
    OP_UPDATE = 3,
    OP_DELETE = 4,
};

#define OP_TYPE_COUNT 5
extern const char* op_type_tokens[OP_TYPE_COUNT];

enum OpCreateType {
    OP_CREATE_TABLE = 0,
};

#define OP_CREATE_TYPE_COUNT 1
extern const char* op_create_type_tokens[OP_CREATE_TYPE_COUNT];

enum OpDropType {
    OP_DROP_TABLE = 0,
};

#define OP_DROP_TYPE_COUNT 1
extern const char* op_drop_type_tokens[OP_DROP_TYPE_COUNT];

// ---- Structures ----

struct OpCreate;
struct OpDrop;

struct Operation {
    enum OpType type;
    union {
        struct OpCreate* create;
        struct OpDrop* drop;
    };
};

void operationFree(struct Operation* operation);

enum TableType {
    TABLE_TYPE_BTREE = 0,
    TABLE_TYPE_HASH = 1,
    TABLE_TYPE_RECNO = 2
};

struct OpCreate {
    enum TableType type;
    char* table_name;
    size_t table_name_len;
};

void opCreateFree(struct OpCreate* op_create);

struct OpDrop {
    char* table_name;
    size_t table_name_len;
};

void opDropFree(struct OpDrop* op_drop);

#endif // _UNIX_DB_QUERY_H_
