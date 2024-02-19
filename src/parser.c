#include "parser.h"
#include "diag.h"

Parser parser_init(Lexer lexer){
    return (Parser){
        .lexer = lexer,
        .current_token = lexer_next_token(&lexer)
    };
}

void parser_next_token(Parser *parser){
    parser->current_token = lexer_next_token(&parser->lexer);
}

void parser_expect_and_advance(Parser *parser, Token_Type type){
    assert(parser);
    if(parser->current_token.type != type){
        // TODO: Better error for this
        lang_write_error(parser->lexer.loc, "Unexpected token %d!\n", (int)type);
    }
    parser_next_token(parser);
}

typedef struct{
    String name;
    Type_Kind kind;
} Types;

const Types types[] = {
    {.name = SV_STATIC("String"), .kind = LANG_TYPE_KIND_STRING},
};

static Type_Kind parse_type_decleration(Parser *parser){
    for(int i = 0; i < (int)(sizeof(types)/sizeof(types[0])); ++i){
        if(sv_eq(parser->current_token.string_value, types[i].name)){
            return types[i].kind;
        }
    }
    lang_write_error(parser->current_token.loc, "Unknown type name `" SV_Fmt "`\n", SV_Arg(parser->current_token.string_value));
    return 0;
}

static bool parser_at(Parser *parser, Token_Type type){
    return parser->current_token.type == type;
}

static Node* parse_func_decl(Parser *parser){
    assert(parser);
    parser_expect_and_advance(parser, LANG_TOKEN_FUNC);
    Node* node = node_init(LANG_NODE_DECL_FUNC, parser->current_token.loc);
    node->decl_func.name = parser->current_token.string_value;
    parser_expect_and_advance(parser, LANG_TOKEN_OPENPAREN);
    // TODO: Parse function arguments
    while(!parser_at(parser, LANG_TOKEN_CLOSEPAREN)){
        Type_Kind type = parse_type_decleration(parser);
        Node* param = node_init(LANG_NODE_DECL_FUNC_PARAM, parser->lexer.loc);
        node->decl_func_param.name = parser->current_token.string_value;
        node->decl_func_param.type = type;
        parser_expect_and_advance(parser, LANG_TOKEN_IDENTIFIER);
        LIST_PUSH(node->decl_func.arguments, param);
    }
    parser_expect_and_advance(parser, LANG_TOKEN_CLOSEPAREN);
    return node;
    // node->decl_func.body
}

static Node* parse_func_or_variable_expression(Parser *parser){
    assert(parser);
    if(parser->current_token.type == LANG_TOKEN_FUNC){
        return parse_func_decl(parser);
    } else{
        lang_write_error(parser->current_token.loc, "Invalid token while parsing function or variable decleration! `" SV_Fmt "`\n", SV_Arg(parser->current_token.string_value));
        exit(1);
    }
}

Node* parser_next_node(Parser *parser){
    assert(parser);
    switch(parser->current_token.type){
        case LANG_TOKEN_EXTERN: {
            parser_next_token(parser);
            Node* node = parse_func_or_variable_expression(parser);
            if(node->type == LANG_NODE_DECL_FUNC){
                node->decl_func.linkage = LANG_LINKAGE_EXTERNAL;
            }
            return node;
        } break;
        case LANG_TOKEN_FUNC: {
            return parse_func_decl(parser);
        } break;
    }
}