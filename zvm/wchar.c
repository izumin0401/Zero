#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "DBG.h"
#include "MEM.h"
#include "zvm_pri.h"

wchar_t *
zvm_mbstowcs_alloc(ZVM_VirtualMachine *zvm, const char *src)
{
    int len;
    wchar_t *ret;

    len = zvm_mbstowcs_len(src);
    if (len < 0) {
        if (zvm) {
            zvm_error_i(zvm->current_executable->executable,
                        zvm->current_function,
                        zvm->pc,
                        BAD_MULTIBYTE_CHARACTER_ERR,
                        ZVM_MESSAGE_ARGUMENT_END);
        } else {
            zvm_error_i(NULL, NULL, NO_LINE_NUMBER_PC,
                        BAD_MULTIBYTE_CHARACTER_ERR,
                        ZVM_MESSAGE_ARGUMENT_END);
        }
        return NULL;
    }
    ret = MEM_malloc(sizeof(wchar_t) * (len+1));
    zvm_mbstowcs(src, ret);

    return ret;
}

char *
ZVM_wcstombs(const wchar_t *src)
{
    char *ret;

    ret = zvm_wcstombs_alloc(src);

    return ret;
}

ZVM_Char *
ZVM_mbstowcs(const char *src)
{
    int len;
    ZVM_Char *ret;

    len = zvm_mbstowcs_len(src);
    if (len < 0) {
        return NULL;
    }
    ret = MEM_malloc(sizeof(wchar_t) * (len+1));
    zvm_mbstowcs(src, ret);

    return ret;
}
