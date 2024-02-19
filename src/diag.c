#include "diag.h"
#include "sv.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

String string_vformat(char* fmt, va_list v){
    va_list v1;
    va_copy(v1, v);
    int n = vsnprintf(NULL, 0, fmt, v1);
    va_end(v1);

    char* result = malloc((size_t)n + 1);
    vsnprintf(result, n + 1, fmt, v);

    char* buffer = result;
    int64_t count = strlen(buffer);
    return sv_from_parts(buffer, count);
}

static void print_location(Location loc){
    printf(SV_Fmt ":%ld: ", SV_Arg(loc.file_name), loc.offset);
}

void lang_write_error_without_loc(char* fmt, ...){
    va_list v;
    va_start(v, fmt);
    String message = string_vformat(fmt, v);
    va_end(v);
    printf("Fatal error: ");
    printf(SV_Fmt, SV_Arg(message));
    free((void*)message.data);
    exit(1);
}

void lang_write_error(Location location, char* fmt, ...){
    print_location(location);
    va_list v;
    va_start(v, fmt);
    String message = string_vformat(fmt, v);
    va_end(v);
    printf("Fatal error: ");
    printf(SV_Fmt, SV_Arg(message));
    free((void*)message.data);
    exit(1);
}

void lang_print_node(Node node){
    print_location(node.loc);
    printf("Type: %d\n", (int)node.type);
}