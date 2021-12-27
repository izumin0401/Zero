#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

void
zvm_vstr_clear(VString *v)
{
    v->string = NULL;
}

static int
my_strlen(ZVM_Char *str)
{
    if (str == NULL) {
        return 0;
    }
    return zvm_wcslen(str);
}

void
zvm_vstr_append_string(VString *v, ZVM_Char *str)
{
    int new_size;
    int old_len;

    old_len = my_strlen(v->string);
    new_size = sizeof(ZVM_Char) * (old_len + zvm_wcslen(str)  + 1);
    v->string = MEM_realloc(v->string, new_size);
    zvm_wcscpy(&v->string[old_len], str);
}

void
zvm_vstr_append_character(VString *v, ZVM_Char ch)
{
    int current_len;
    
    current_len = my_strlen(v->string);
    v->string = MEM_realloc(v->string,sizeof(ZVM_Char) * (current_len + 2));
    v->string[current_len] = ch;
    v->string[current_len+1] = L'\0';
}

void
zvm_initialize_value(ZVM_TypeSpecifier *type, ZVM_Value *value)
{
    if (type->derive_count > 0) {
        if (type->derive[0].tag == ZVM_ARRAY_DERIVE) {
            value->object = zvm_null_object_ref;
        } else {
            DBG_assert(0, ("tag..%d", type->derive[0].tag));
        }
    } else {
        switch (type->basic_type) {
        case ZVM_VOID_TYPE:  /* FALLTHRU */
        case ZVM_BOOLEAN_TYPE: /* FALLTHRU */
        case ZVM_INT_TYPE: /* FALLTHRU */
        case ZVM_ENUM_TYPE: /* FALLTHRU */
            value->int_value = 0;
            break;
        case ZVM_DOUBLE_TYPE:
            value->double_value = 0.0;
            break;
        case ZVM_STRING_TYPE: /* FALLTHRU */
        case ZVM_NATIVE_POINTER_TYPE: /* FALLTHRU */
        case ZVM_CLASS_TYPE: /* FALLTHRU */
        case ZVM_DELEGATE_TYPE:
            value->object = zvm_null_object_ref;
            break;
        case ZVM_NULL_TYPE: /* FALLTHRU */
        case ZVM_BASE_TYPE: /* FALLTHRU */
        case ZVM_UNSPECIFIED_IDENTIFIER_TYPE: /* FALLTHRU */
        default:
            DBG_assert(0, ("basic_type..%d", type->basic_type));
        }
    }
}
