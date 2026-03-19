#include "table.h"

#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>

int tableMetadataDeserialize(uint8_t* buf, size_t buf_len, struct TableMetadata* meta) {
    const size_t original_buf_len = buf_len;
    uint8_t byte;
    int error = parseU8(&buf, &buf_len, &byte);
    if (error) {
        return error;
    }
    if (byte < 0 || byte > TABLE_TYPE_BTREE) {
        fprintf(stderr, "Invalid table type: %d\n", (int) byte);
        return -ERROR_TABLE_TYPE_INVALID;
    }
    const enum TableType type = (enum TableType) byte;

    size_t name_len;
    error = parseSizet(&buf, &buf_len, &name_len);
    if (error) {
        return error;
    }

    char* name = NULL;
    error = parseString(&buf, &buf_len, name_len, &name);
    if (error) {
        return error;
    }

    size_t data_file_path_len;
    error = parseSizet(&buf, &buf_len, &data_file_path_len);
    if (error) {
        return error;
    }

    char* data_file_path = NULL;
    error = parseString(&buf, &buf_len, data_file_path_len, &data_file_path);
    if (error) {
        return error;
    }

    *meta = (struct TableMetadata) {
        .type = type,
        .name_len = name_len,
        .name = name,
        .data_file_path_len = data_file_path_len,
        .data_file_path = data_file_path,
    };
    return original_buf_len - buf_len;
}

int tableMetadataSerialize(struct TableMetadata* meta, uint8_t* buf) {
    int error;
    if ((error = writeU8(buf, (char) meta->type))) {
        return error;
    }
    if ((error = writeSizet(buf, meta->name_len))) {
        return error;
    } 
    if ((error = writeString(buf, meta->name, meta->name_len))) {
        return error;
    }
    if ((error = writeSizet(buf, meta->data_file_path_len))) {
        return error;
    }
    if ((error = writeString(buf, meta->data_file_path, meta->data_file_path_len))) {
        return error;
    }
    return 0;
}

int tableMetadataSizeBytes(struct TableMetadata* meta) {
    size_t bytes_count = 1;
    bytes_count += bytesof(size_t);
    bytes_count += meta->name_len * bytesof(char);
    bytes_count += bytesof(size_t);
    bytes_count += meta->data_file_path_len * bytesof(char);
    return bytes_count;
}

void tableMetadataFree(struct TableMetadata* meta) {
    free(meta->name);
    free(meta->data_file_path);
}
