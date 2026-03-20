#pragma once

#ifndef _UNIX_DB_TABLE_H_
#define _UNIX_DB_TABLE_H_

#include <stddef.h>
#include <stdint.h>

#include "operation.h"

#define ERROR_TABLE_TYPE_INVALID 0x1
#define ERROR_TABLE_TYPE_BUF_TOO_SMALL 0x2

struct TableMetadata {
    enum TableType type;
    size_t name_len;
    char* name;
    size_t data_file_path_len;
    char* data_file_path;
};

// Saturates `meta`
// Returns count of bytes parsed from buf
// otherwise -1 on error
int tableMetadataDeserialize(char* buf, size_t buf_len, struct TableMetadata* meta);
// Assumes buf contains enough space
// to serialize the table metadata.
// The size required can be acquired
// from tableMetadataSizeBytes
int tableMetadataSerialize(struct TableMetadata* meta, char* buf);

int tableMetadataSizeBytes(struct TableMetadata* meta);
void tableMetadataFree(struct TableMetadata* meta);

#endif // _UNIX_DB_TABLE_H_ 
