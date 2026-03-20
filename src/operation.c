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

size_t operationParseType(char* buf, size_t buf_len, enum OperationType* out_type) {
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
