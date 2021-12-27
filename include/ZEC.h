#ifndef PUBLIC_ZEC_H_INCLUDED
#define PUBLIC_ZEC_H_INCLUDED
#include <stdio.h>
#include "ZVM_code.h"

typedef struct ZEC_Compiler_tag ZEC_Compiler;

ZEC_Compiler *ZEC_create_compiler(void);
ZVM_ExecutableList *ZEC_compile(ZEC_Compiler *compiler, FILE *fp, char *path);
void ZEC_dispose_compiler(ZEC_Compiler *compiler);

#endif /* PUBLIC_ZEC_H_INCLUDED */
