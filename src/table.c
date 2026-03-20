#include "table.h"

#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>

int tableMetadataDeserialize(char* buf, size_t buf_len, struct TableMetadata* meta) {
    const size_t original_buf_len = buf_len;
    uint8_t byte;
    int byte_count = parseU8(buf, buf_len, &byte);
    if (byte_count <= 0) {
        fprintf(stderr, "Failed to parse type\n");
        return byte_count;
    }
    shiftBuf(buf, buf_len, byte_count);
    if (byte < 0 || byte > TABLE_TYPE_RECNO) {
        fprintf(stderr, "Invalid table type: %d\n", (int) byte);
        return -ERROR_TABLE_TYPE_INVALID;
    }
    const enum TableType type = (enum TableType) byte;

    size_t name_len;
    byte_count = parseSizet(buf, buf_len, &name_len);
    if (byte_count <= 0) {
        fprintf(stderr, "Failed to parse name length\n");
        return byte_count;
    }
    shiftBuf(buf, buf_len, byte_count);

    char* name = NULL;
    byte_count = parseString(buf, buf_len, name_len, &name);
    if (byte_count <= 0) {
        fprintf(stderr, "Failed to parse name\n");
        return byte_count;
    }
    shiftBuf(buf, buf_len, byte_count);

    size_t data_file_path_len;
    byte_count = parseSizet(buf, buf_len, &data_file_path_len);
    if (byte_count <= 0) {
        fprintf(stderr, "Failed to parse path length\n");
        return byte_count;
    }
    shiftBuf(buf, buf_len, byte_count);

    char* data_file_path = NULL;
    byte_count = parseString(buf, buf_len, data_file_path_len, &data_file_path);
    if (byte_count <= 0) {
        fprintf(stderr, "Failed to parse path\n");
        return byte_count;
    }
    shiftBuf(buf, buf_len, byte_count);

    *meta = (struct TableMetadata) {
        .type = type,
        .name_len = name_len,
        .name = name,
        .data_file_path_len = data_file_path_len,
        .data_file_path = data_file_path,
    };
    return original_buf_len - buf_len;
}

int tableMetadataSerialize(struct TableMetadata* meta, char* buf) {
    int error;
    if ((error = writeU8(buf, (char) meta->type))) {
        return error;
    }
    buf += 1;
    if ((error = writeSizet(buf, meta->name_len))) {
        return error;
    } 
    buf += sizeof(size_t);
    if ((error = writeString(buf, meta->name, meta->name_len))) {
        return error;
    }
    buf += meta->name_len;
    if ((error = writeSizet(buf, meta->data_file_path_len))) {
        return error;
    }
    buf += sizeof(size_t);
    if ((error = writeString(buf, meta->data_file_path, meta->data_file_path_len))) {
        return error;
    }
    return 0;
}

int tableMetadataSizeBytes(struct TableMetadata* meta) {
    size_t bytes_count = 1;
    bytes_count += sizeof(size_t);
    bytes_count += meta->name_len * sizeof(char);
    bytes_count += sizeof(size_t);
    bytes_count += meta->data_file_path_len * sizeof(char);
    return bytes_count;
}

void tableMetadataFree(struct TableMetadata* meta) {
    free(meta->name);
    free(meta->data_file_path);
}
