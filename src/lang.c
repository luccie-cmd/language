#include "lang.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "diag.h"
#include "token.h"

command_line_args command_line_args_init(int argc, char **argv){
    command_line_args args = {argc, argv, NULL};
    return args;
}

void command_line_args_extract_args(command_line_args *args){
    for(int i = 1; i < args->argc; ++i){
        char* arg = args->argv[i];
        args->input_file = arg;
    }
    if(args->input_file == NULL){
        printf("No valid input file provided!\n");
        exit(1);
    }
}

String ReadFile(char* file){
    FILE* f = fopen(file, "rb");
    if(!f){
        lang_write_error_without_loc("%s: %s\n", file, strerror(errno));
    }

    fseek(f, 0, SEEK_END);
    size_t len = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);

    char* data = malloc(len);
    if(!data){
        lang_write_error_without_loc("ReadFile: %s\n", file, strerror(errno));
    }
    fread((void*)data, 1, len, f);

    return sv_from_parts(data, len);
}

Location init_location(String file){
    return (Location){
        .file_name = file,
        .offset = 0
    };
}
void advance_location(Location *loc, size_t N){
    loc->offset+=N;
}

Node *node_init(Node_Type type, Location loc){
    Node* node = malloc(sizeof(Node));
    node->loc = loc;
    node->type = type;
    return node;
}

Token token_init(Location loc, String value, Token_Type type){
    return (Token){
        .loc = loc,
        .string_value = value,
        .type = type
    };
}