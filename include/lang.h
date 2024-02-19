#if !defined(_LANGUAGE_H_)
#define _LANGUAGE_H_
#include "sv.h"
#include "list.h"
#include <assert.h>

typedef struct command_line_args{
    int argc;
    char **argv;

    char* input_file;
} command_line_args;

typedef struct{
    String file_name;
    size_t offset;
} Location;

command_line_args command_line_args_init(int argc, char **argv);
void command_line_args_extract_args(command_line_args *args);

String ReadFile(char* file);

Location init_location(String file);
void advance_location(Location *loc, size_t N);

// Parser/Ast stuff
typedef enum{
    LANG_TYPE_KIND_STRING,
} Type_Kind;
typedef enum{
    LANG_LINKAGE_EXTERNAL,
    LANG_LINKAGE_GLOBAL,
} Linkage;
typedef enum{
    LANG_NODE_DECL_FUNC,
    LANG_NODE_COMPOUND,
    LANG_NODE_DECL_FUNC_PARAM,
} Node_Type;
typedef struct Node Node;
struct Node{
    Node_Type type;
    Location loc;
    union{
        struct{
            String name;
            Linkage linkage;
            Node *body; // Should be a compound node
            LIST(Node*) arguments;
        } decl_func;
        struct{
            LIST(Node*) child_nodes;
        } compound;
        struct{
            String name;
            Type_Kind type;
        } decl_func_param;
    };
};

Node *node_init(Node_Type type, Location loc);

#endif // _LANGUAGE_H_
