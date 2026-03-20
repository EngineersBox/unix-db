#pragma once

#ifndef _UNIX_DB_QUERY_H_
#define _UNIX_DB_QUERY_H_

#include <stddef.h>

enum OperationType {
    OP_CREATE,
    OP_DROP,
    OP_SELECT,
    OP_UPDATE,
    OP_DELETE,
};

size_t operationParseType(char* buf, size_t buf_len, OperationType* out_type);

#endif // _UNIX_DB_QUERY_H_
