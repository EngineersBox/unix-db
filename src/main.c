#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <db.h>
#include <string.h>

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

int main(const int argc, const char** argv) {
    const char* table_file = "./test_data/table.db";
    const HASHINFO hash_info = (HASHINFO) {
        .bsize = 256,
        .ffactor = 8,
        .nelem = 1,
        .cachesize = 1024 * 4,
        .hash = &hash_func,
        .lorder = 1234,
    };
    DB* db = dbopen(
        table_file,
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
