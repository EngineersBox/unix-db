#include "lexer.h"
#include "math_utils.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

const char* token_names[] = {
	[STRING] = "STRING",
    [COMMA] = "COMMA",
    [SEMICOLON] = "SEMICOLON",
	[EOI] = "EOI"
};

struct Lexer lexerNew(char* source, size_t str_len) {
	struct Lexer lexer = {0};
	lexer.source = NULL;
	lexer.string = NULL;
	lexerReset(&lexer, source, str_len);
	return lexer;
}

void lexerFree(struct Lexer* lexer) {
	if (lexer == NULL) {
		return;
	}
    if (lexer->string != NULL) free(lexer->string);
    if (lexer->source != NULL) free(lexer->source);
}

int lexerReset(struct Lexer* lexer, char* source, size_t str_len) {
  if (lexer->source != NULL) {
      free(lexer->source);
  }
  lexer->source = strdup(source);
  lexer->pos = 0;
  lexer->symbol = -1;
  lexer->source_len = str_len;
  if (lexer->source_len == 0) {
    fprintf(stderr, "Source must not be empty\n");
    return -EINVAL;
  }
	lexer->cchar = source[0];
	lexer->is_reading_string = false;
	lexer->string_len = 0;
	lexer->string_pos = 0;
	if (lexer->string != NULL) {
		free(lexer->string);
		lexer->string = NULL;
	}
	return 1;
}

void lexerPrintState(struct Lexer* lexer) {
	fprintf(
		stderr,
		"STATE: {\n\tPOS: %zu\n\tCHAR: %c\n\tSYMBOL: %s\n\tSOURCE LEN: %zu\n\tSOURCE: %s\n\tIS READING STRING: %s\n\tSTRING LEN: %zu\n\tSTRING POS: %zu\n\tSTRING:%s\n}\n",
		lexer->pos,
		lexer->cchar,
		token_names[lexer->symbol],
		lexer->source_len,
		lexer->source,
		lexer->is_reading_string ? "true" : "false",
		lexer->string_len,
		lexer->string_pos,
		lexer->string
	);
}

// 0: Finished, 1: Ignore, Negative: failure
static int nextString(struct Lexer* lexer) {
	if (lexer->pos < lexer->string_pos) {
		return 0;
	}
    if (lexer->string != NULL) free(lexer->string);
	size_t start = lexer->string_pos;
	size_t end = DEC_FLOOR(lexer->pos);
	lexer->string = strndup(&lexer->source[start], end - start);
	lexer->string_len = end - start;
	if (lexer->string == NULL) {
		fprintf(stderr, "Unable to extract string in tokenised sequence\n");
		return -ENOMEM;
	}
	lexer->is_reading_string = false;
	return 1;
}

static void nextChar(struct Lexer* _this) {
	_this->cchar = _this->source[_this->pos++];
}

#define HANDLE_NEXT_STRING \
	if (lexer->is_reading_string) { \
		res = nextString(lexer); \
		if (res == 1) { \
			break; \
		} else if (res != 1) { \
			return res; \
		} \
	}

#define SINGLE_TOKEN_CASE(literal, token, skipSequential) \
	case literal:\
		HANDLE_NEXT_STRING;\
		nextChar(lexer);\
		lexer->symbol = (token);\
		if (skipSequential && lexer->pos < lexer->source_len - 1 && lexer->source[lexer->pos + 1] == literal) {\
			goto read;\
		}\
		break

static void removeChar(struct Lexer* lexer) {
	memmove(&lexer->source[lexer->pos - 1], &lexer->source[lexer->pos], lexer->source_len - lexer->pos + 1);
	lexer->source_len--;
	lexer->pos--;
	nextChar(lexer);
}

int lexerNextSymbol(struct Lexer* lexer) {
    if (lexer->source == NULL) {
        return -1;
    }
    if (lexer->pos >= lexer->source_len - 1) {
        lexer->symbol = EOI;
        return 0;
    }
	int res;
	bool escape = false;
	bool single_quoted = false;
	bool double_quoted = false;
read:
	switch (lexer->cchar) {
		case '\0':
			HANDLE_NEXT_STRING;
			lexer->symbol = EOI;
			break;
		case ' ':
		case '\t':
		case '\n':
			if (escape) {
				nextChar(lexer);
				escape = false;
				goto read;
			}
			if (single_quoted || double_quoted) {
				goto string;
			}
			HANDLE_NEXT_STRING
			nextChar(lexer);
			goto read;
		SINGLE_TOKEN_CASE(EOF, EOI, false);
		SINGLE_TOKEN_CASE(_TOK_COMMA, COMMA, true);
		SINGLE_TOKEN_CASE(_TOK_SEMICOLON, SEMICOLON, false);
		case '\\':
			escape = true;
			goto string;
		case '"':
			if (!escape) {
				double_quoted = !double_quoted;
				if (!double_quoted) {
					removeChar(lexer);
				} else {
					nextChar(lexer);
				}
				goto read;
			}
			goto string;
		case '\'':
			if (!escape) {
				single_quoted = !single_quoted;
				if (!double_quoted) {
					removeChar(lexer);
				} else {
					nextChar(lexer);
				}
				goto read;
			}
			goto string;
		default:
string:
			if (lexer->is_reading_string) {
				nextChar(lexer);
				goto read;
			}
			lexer->string_len = 0;
			lexer->string_pos = DEC_FLOOR(lexer->pos);
			lexer->is_reading_string = true;
			lexer->symbol = STRING;
			nextChar(lexer);
			goto read;
	}
	return 1;
}

int lexerCurrentSymbol(struct Lexer* lexer) {
    if (lexer->source == NULL) return -1;
    return lexer->symbol;
}

char* lexerCurrentString(struct Lexer* lexer) {
    if (lexer->source == NULL) return NULL;
    return lexer->string;
}
