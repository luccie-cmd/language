#include <stdio.h>
#include "lang.h"
#include "sv.h"
#include "lexer.h"
#include "parser.h"
#include "diag.h"

int main(int argc, char **argv){
    command_line_args cmd_args = command_line_args_init(argc, argv);
    command_line_args_extract_args(&cmd_args);
    String file_data = ReadFile(cmd_args.input_file);

    Lexer lexer = lexer_init(sv_from_cstr(cmd_args.input_file), file_data);
    Parser parser = parser_init(lexer);
    Node *node = parser_next_node(&parser);
    // lang_print_node(node);

    free(node);
    free((void*)file_data.data);
    return 0;
}