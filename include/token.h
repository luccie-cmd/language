#if !defined(_LANGUAGE_TOKEN_H_)
#define _LANGUAGE_TOKEN_H_
#include "lang.h"

typedef enum{
    LANG_TOKEN_OPENPAREN = '(',
    LANG_TOKEN_CLOSEPAREN = ')',
    LANG_TOKEN_OPENBRACE = '{',
    LANG_TOKEN_CLOSEBRACE = '}',
    LANG_TOKEN_SEMICOLON = ';',
    LANG_TOKEN_PERCENT = '%',

    LANG_TOKEN_LITSTRING,
    LANG_TOKEN_FUNC,
    LANG_TOKEN_IDENTIFIER,
    LANG_TOKEN_RETURN,
    LANG_TOKEN_LITINT,
    LANG_TOKEN_EXTERN,
} Token_Type;

typedef struct{
    Location loc;
    Token_Type type;
    union{
        int int_value;
        String string_value;
    };
} Token;

Token token_init(Location loc, String value, Token_Type type);

#endif // _LANGUAGE_TOKEN_H_
