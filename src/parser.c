#include "parser.h"

#include "lexer.h"

#include <stdio.h>
#include <string.h>

static int parseOpCreate(struct Lexer* lexer, struct OpCreate* op_create) {
    return 0;
}

static int parseOpDrop(struct Lexer* lexer, struct OpDrop* op_drop) {
    return 0;
}

static int parseOperation(struct Lexer* lexer, struct Operation* operation) {
    Token symbol = lexerCurrentSymbol(lexer);
    if (symbol != STRING) {
        fprintf(stderr, "Expected operation type, got %s\n", token_names[symbol]);
        return -1;
    }
    enum OpType type = -1;
    for (int i = 0; i < OP_TYPE_COUNT; i++) {
        const char* op_type = op_type_tokens[i];
        if (strncmp(lexerCurrentString(lexer), op_type, strlen(op_type)) == 0) {
            type = (enum OpType) i;
            break;
        }
    }
    if (type == -1) {
        fprintf(stderr, "Invalid operation type %s\n", lexerCurrentString(lexer));
        return -1;
    }
    operation->type = type;
    switch (type) {
        case OP_CREATE:
            operation->create = malloc(sizeof(struct OpCreate));
            return parseOpCreate(lexer, operation->create);
            break;
        case OP_DROP:
            operation->drop = malloc(sizeof(struct OpDrop));
            return parseOpDrop(lexer, operation->drop);
            break;
        case OP_SELECT:
            break;
        case OP_UPDATE:
            break;
        case OP_DELETE:
            break;
        default:
            fprintf(stderr, "Missing operation type parser implementation for %s\n", lexerCurrentString(lexer));
            return -1;
    }
    return !lexerNextSymbol(lexer) && lexerCurrentSymbol(lexer) == SEMICOLON;
}

int parseOperations(char* buf, size_t buf_len, cvector(struct Operation)* out_tokens) {
    struct Lexer lexer = lexerNew(buf, buf_len);
    Token symbol;
    struct Operation operation;
    while (lexerNextSymbol(&lexer) && (symbol = lexerCurrentSymbol(&lexer)) != EOI) {
        if (symbol != STRING) {
            fprintf(stderr, "Expected operation type\n");
            lexerFree(&lexer);
            return 1;
        } else if (parseOperation(&lexer, &operation)) {
            fprintf(stderr, "Failed to parse operation\n");
            lexerFree(&lexer);
            return 1;
        }
        cvector_push_back_safe(out_tokens, operation);
    }
    lexerFree(&lexer);
    return 0;
}
