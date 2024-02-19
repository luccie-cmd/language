#if !defined(_LANGUAGE_LEXER_H_)
#define _LANGUAGE_LEXER_H_
#include "lang.h"
#include "token.h"

typedef struct{
    Location loc;

    String data;
} Lexer;

Lexer lexer_init(String file, String file_data);
Token lexer_next_token(Lexer *lexer);

#endif // _LANGUAGE_LEXER_H_
