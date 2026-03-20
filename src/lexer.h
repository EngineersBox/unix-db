#ifndef _UNIX_DB_LEXER_H_
#define _UNIX_DB_LEXER_H_

#include <stdbool.h>
#include <stddef.h>

#define _TOK_COMMA ','
#define _TOK_SEMICOLON ';'

#define _IS_RESERVED(c) (c) == _TOK_COMMA || (c) == _TOK_SEMICOLON
#define _IS_WHITESPACE(c) (c) == ' ' || (c) == '\t' || (c) == '\n'
#define _IS_STRIPPABLE(c) (c) == '"' || (c) == '\''

typedef enum Token {
	STRING = 0,
    COMMA = 2,
    SEMICOLON = 3,
	EOI = 4,
} Token;

extern const char* token_names[];

struct Lexer {
	size_t pos;
	int cchar;
	int symbol;
	size_t source_len;
	char* source;
	bool is_reading_string;
	size_t string_len;
	size_t string_pos;
	char* string;
};

struct Lexer lexerNew(char* source, size_t str_len);
void lexerFree(struct Lexer* lexer);

int lexerReset(struct Lexer* lexer, char* source, size_t str_len);
int lexerNextSymbol(struct Lexer* lexer);

int lexerCurrentSymbol(struct Lexer* lexer);
char* lexerCurrentString(struct Lexer* lexer);

void lexerPrintState(struct Lexer* lexer);

#endif // _UNIX_DB_LEXER_H_
