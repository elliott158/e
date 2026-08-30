#include <stdio.h>
#include <stdlib.h>

#define da_append(xs, x)\
    do {\
        if (xs.used >= xs.capacity) {\
            if (xs.capacity == 0) xs.capacity = 256;\
            else xs.capacity *= 2;\
            xs.items = realloc(xs.items, xs.capacity*sizeof(*xs.items));\
        }\
        xs.items[xs.used++] = x;\
    } while (0)

#define da_free(xs)\
  do {\
    free(xs.items);\
    xs.items = nullptr;\
    xs.used = 0;\
    xs.capacity=0;\
  } while(0)
