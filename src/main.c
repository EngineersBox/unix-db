#include "parsing.h"
#include "table.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <db.h>
#include <string.h>
#include <unistd.h>

uint32_t hash_func(const void* data, size_t size) {
    if (size < 1) {
        return 0;
    } else if (size < sizeof(void*)) {
        uint32_t value = ((char*) data)[0];
        for (int i = 1; i < size; i++) {
            value ^= ((char*) data)[i];
        }
        return value;
    }
    const uint32_t* u32_data = (uint32_t*) data;
    const size_t u32_count = size / sizeof(uint32_t);
    uint32_t value = u32_data[0];
    for (int i = 1; i < u32_count; i++) {
        value ^= u32_data[i];
    }
    const size_t base = u32_count * sizeof(uint32_t);
    const size_t excess = size - base;
    if (excess == 0) {
        return value;
    }
    const char* char_data = (char*) &u32_data[u32_count];
    for (int i = base; i < base + excess; i++) {
        value ^= char_data[i];
    }
    return value;
}

int main1(const int argc, const char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s: <table file path>\n", argv[0]);
        return 1;
    }
    const HASHINFO hash_info = (HASHINFO) {
        .bsize = 256,
        .ffactor = 8,
        .nelem = 1,
        .cachesize = 1024 * 4,
        .hash = &hash_func,
        .lorder = 1234,
    };
    printf("Opening table %s\n", argv[1]);
    DB* db = dbopen(
        argv[1],
        O_CREAT | O_RDWR,
        0777,
        DB_HASH,
        &hash_info
    );
    if (db == NULL) {
        perror("Failed to create db");
        return errno;
    }
    DBT key = (DBT) {
        .data = "test this thing with other thing yeah",
        .size = sizeof(char) * 37,
    };
    const DBT value = (DBT) {
        .data = "some result data",
        .size = sizeof(char) * 16,
    };
    int result = db->put(db, &key, &value, R_NOOVERWRITE);
    if (result) {
        printf("Entry already exists for key '%s', skipping\n", (char*) key.data);
    } else {
        result = db->sync(db, 0);
        if (result) {
            perror("Failed to sync db table");
            return errno;
        }
    }
    printf("Stored key '%s' with value '%s'\n", (char*) key.data, (char*) value.data);
    DBT value_get = (DBT) {0};
    result = db->get(db, &key, &value_get, 0);
    if (result) {
        fprintf(stderr, "Failed to retrieve data keyed on %s: %s\n", (char*) key.data, strerror(errno));
        return errno;
    }
    char str_value[value_get.size];
    memcpy(str_value, value_get.data, value_get.size);
    printf("Retrieved key '%s' with value '%s'\n", (char*) key.data, str_value);
    return db->close(db);
}

int main(const int argc, const char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <in | out>\n", argv[0]);
        return 1;
    }
    char* path = "./test_data/table.meta";
    if (strncmp(argv[1], "out", 3) == 0) {
        printf("Writing data out\n");
        struct TableMetadata meta = (struct TableMetadata) {0};
        meta.type = TABLE_TYPE_HASH;
        meta.name = "test";
        meta.name_len = strlen(meta.name);
        meta.data_file_path = "./test_data/table.db";
        meta.data_file_path_len = strlen(meta.data_file_path);
        int fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0641);
        if (fd < 0) {
            perror("Failed to create table metadata file");
            return 1;
        }
        const size_t buf_size = tableMetadataSizeBytes(&meta);
        char* buf = calloc(buf_size, sizeof(char));
        if (tableMetadataSerialize(&meta, buf)) {
            fprintf(stderr, "Failed to serialize table metadata\n");
            close(fd);
            return 1;
        }
        char buf_size_buf[sizeof(size_t)];
        writeSizet(buf_size_buf, buf_size);
        if (write(fd, buf_size_buf, sizeof(size_t)) == -1) {
            perror("Failed to write table metadata size to file");
            close(fd);
            return 1;
        }
        if (write(fd, (void*) buf, buf_size) == -1) {
            perror("Failed to write table metadata to file");
            close(fd);
            return 1;
        }
        close(fd);
        return 0;
    }
    printf("Reading data in\n");
    int fd = open(path, O_RDONLY);
    if (fd < 1) {
        perror("Failed to open table metadata file");
        return 1;
    }
    lseek(fd, 0, SEEK_SET);
    char buf_size_buf[sizeof(size_t)];
    if (read(fd, buf_size_buf, sizeof(size_t)) <= 0) {
        perror("Failed to read table metadata size from file");
        return 1;
    }
    size_t buf_size = atoi((char*) buf_size_buf);
    struct TableMetadata meta = {0};
    char buf[buf_size];
    size_t buf_len;
    if ((buf_len = read(fd, buf, buf_size)) <= 0) {
        perror("Failed to read table metadata bytes to buffer from file");
        close(fd);
        return 1;
    }
    if (tableMetadataDeserialize(buf, buf_len, &meta) < 0) {
        fprintf(stderr, "Failed to deserialize table metadata\n");
        close(fd);
        return 1;
    }
    printf("[META] Type: %d\n", meta.type);
    printf("[META] Name: %s Len: %zu\n", meta.name, meta.name_len);
    printf("[META] Path: %s Len: %zu\n", meta.data_file_path, meta.data_file_path_len);
    tableMetadataFree(&meta);
    close(fd);
    return 0;
}
