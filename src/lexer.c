#include "lexer.h"
#include "diag.h"

Lexer lexer_init(String file, String data){
    return (Lexer){
        .data = data,
        .loc = init_location(file)
    };
}

static bool is_skippable(char c){
    return ((c == ' ' || c == '\n' || c == '\r' || c == '\t') && c != '\0');
}

static bool is_identifier(char c){
    return (('z' >= c && c >= 'a') || ('Z' >= c && c >= 'A') || c == '_');
}

static void lexer_skip_whitespace(Lexer *lexer){
    assert(lexer);
    size_t prev_count = lexer->data.count;
    sv_chop_left_while(&lexer->data, is_skippable);
    size_t current_count = lexer->data.count;
    advance_location(&lexer->loc, prev_count-current_count);
}

const Token single_keyword_tokens[] = {
    { .type = LANG_TOKEN_OPENPAREN, .string_value = SV_STATIC("(") },
    { .type = LANG_TOKEN_CLOSEPAREN, .string_value = SV_STATIC(")") },
    { .type = LANG_TOKEN_OPENBRACE, .string_value = SV_STATIC("{") },
    { .type = LANG_TOKEN_CLOSEBRACE, .string_value = SV_STATIC("}") },
    { .type = LANG_TOKEN_SEMICOLON, .string_value = SV_STATIC(";") },
    { .type = LANG_TOKEN_PERCENT, .string_value = SV_STATIC("%%") },
    { .type = LANG_TOKEN_FUNC, .string_value = SV_STATIC("func") },
    { .type = LANG_TOKEN_RETURN, .string_value = SV_STATIC("return") },
    { .type = LANG_TOKEN_EXTERN, .string_value = SV_STATIC("extern") }
};

Token lexer_next_token(Lexer *lexer){
    assert(lexer);
    // Check for any comments if found goto next line
    size_t prev_count = lexer->data.count;
    if(sv_starts_with(lexer->data, sv_from_cstr("//"))){
        sv_chop_by_delim(&lexer->data, '\n');
        size_t current_count = lexer->data.count;
        advance_location(&lexer->loc, prev_count-current_count);
    }
    // Skip any whitespaces that might be there
    lexer_skip_whitespace(lexer);
    // Look token up in table of tokens
    for(int i = 0; i < (int)(sizeof(single_keyword_tokens)/sizeof(single_keyword_tokens[0])); ++i){
        Token keyword = single_keyword_tokens[i];
        if(sv_starts_with(lexer->data, keyword.string_value)){
            keyword.loc = lexer->loc;
            advance_location(&lexer->loc, keyword.string_value.count);
            sv_chop_by_sv(&lexer->data, keyword.string_value);
            return keyword;
        }
    }
    // Check for any identifiers
    if(is_identifier(*lexer->data.data)){
        String buffer = SV_NULL;
        size_t prev_count = lexer->data.count;
        buffer = sv_chop_left_while(&lexer->data, is_identifier);
        size_t current_count = lexer->data.count;
        Token tok = token_init(lexer->loc, buffer, LANG_TOKEN_IDENTIFIER);
        advance_location(&lexer->loc, prev_count-current_count);
        return tok;
    }
    lang_write_error(lexer->loc, "Invalid token encounterd!\n");
    exit(1);
}