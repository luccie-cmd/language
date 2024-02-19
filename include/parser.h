#if !defined(_LANGUAGE_PARSER_H_)
#define _LANGUAGE_PARSER_H_
#include "lexer.h"

typedef struct{
    Lexer lexer;
    Token current_token;
} Parser;

Parser parser_init(Lexer lexer);
void parser_next_token(Parser *parser);
Node* parser_next_node(Parser *parser);

#endif // _LANGUAGE_PARSER_H_
