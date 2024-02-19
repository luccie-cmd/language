#if !defined(_LANGUAGE_LIST_H_)
#define _LANGUAGE_LIST_H_
#include <stdlib.h>

#define LIST(T) struct { T* data; size_t count, capacity; }
#define LIST_PUSH(V, E) { \
                          if(V.count+1 >= V.capacity){ \
                            V.capacity=(V.capacity+1)*2;  \
                            V.data = realloc(V.data, V.capacity); \
                          } \
                          V.data[V.count++] = E; \
                          }

#endif // _LANGUAGE_LIST_H_
