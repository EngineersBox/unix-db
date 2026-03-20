#pragma once

#ifndef _UNIX_DB_PARSER_H_
#define _UNIX_DB_PARSER_H_

#include <stddef.h>

#include "cvector.h"
#include "operation.h"

/* PARSER TYPE: LR(1) [Left to right, Rightmost derivation, Single token lookahead]
 * PARSER GRAMMAR:
 * =================================================
 * GOAL: OperationList;
 *
 * TableType:
 *      | BTREE
 *      | HASH
 *      | RECNO;
 *
 * CreateOperationArgs:
 *      | TABLE TableType <STRING> <SEMICOLON>;
 *
 * DropOperationArgs:
 *      | TABLE <STRING> <SEMICOLON>;
 *
 * Operation:
 *      | CREATE CreateOperationArgs
 *      | DROP DropOperationArgs;
 *
 * OperationList: Operation*;
 * =================================================
 */

int parseOperations(char* buf, size_t buf_len, cvector(struct Operation)* out_tokens);

#endif // _UNIX_DB_PARSER_H_
