#!/usr/bin/env bash

gcc -Isrc/ -lc -o unix_db src/main.c src/parsing.c src/table.c
