#if !defined(_LANGUAGE_DIAG_H_)
#define _LANGUAGE_DIAG_H_
#include "lang.h"

void lang_write_error_without_loc(char* fmt, ...);
void lang_write_error(Location location, char* fmt, ...);
void lang_print_node(Node node);

#endif // _LANGUAGE_DIAG_H_
