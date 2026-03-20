#include "operation.h"
#include <stdlib.h>

const char* op_type_tokens[OP_TYPE_COUNT] = {
    [OP_CREATE] = "CREATE",
    [OP_DROP] = "DROP",
    [OP_SELECT] = "SELECT",
    [OP_UPDATE] = "UPDATE",
    [OP_DELETE] = "DELETE",
};

const char* op_create_type_tokens[OP_CREATE_TYPE_COUNT] = {
    [OP_CREATE_TABLE] = "TABLE",
};

const char* op_drop_type_tokens[OP_DROP_TYPE_COUNT] = {
    [OP_DROP_TABLE] = "TABLE",
};

void operationFree(struct Operation *operation) {
    switch (operation->type) {
        case OP_CREATE:
            opCreateFree(operation->create);
            break;
        case OP_DROP:
            opDropFree(operation->drop);
            break;
        case OP_SELECT:
            break;
        case OP_UPDATE:
            break;
        case OP_DELETE:
            break;
    }
}

void opCreateFree(struct OpCreate* op_create) {
    free(op_create->table_name);
    free(op_create);
}

void opDropFree(struct OpDrop* op_drop) {
    free(op_drop->table_name);
    free(op_drop);
}
