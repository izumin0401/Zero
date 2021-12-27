#include <stdio.h>
#include <locale.h>
#include "ZEC.h"
#include "ZVM.h"
#include "MEM.h"

int
main(int argc, char **argv)
{
    ZEC_Compiler *compiler;
    FILE *fp;
    ZVM_ExecutableList *list;
    ZVM_VirtualMachine *zvm;

    if (argc < 2) {
        fprintf(stderr, "usage:%s filename arg1, arg2, ...", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }

    setlocale(LC_CTYPE, "");
    compiler = ZEC_create_compiler();
    list = ZEC_compile(compiler, fp, argv[1]);
    zvm = ZVM_create_virtual_machine();
    ZVM_set_executable(zvm, list);
    ZEC_dispose_compiler(compiler);
    ZVM_execute(zvm);
    ZVM_dispose_virtual_machine(zvm);
    ZVM_dispose_executable_list(list);

    MEM_check_all_blocks();
    MEM_dump_blocks(stdout);

    return 0;
}
