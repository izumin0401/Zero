#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

typedef struct {
    ZVM_MessageArgumentType type;
    char        *name;
    union {
        int     int_val;
        double  double_val;
        char    *string_val;
        void    *pointer_val;
        int     character_val;
    } u;
} MessageArgument;

static void
create_message_argument(MessageArgument *arg, va_list ap)
{
    int index = 0;
    ZVM_MessageArgumentType type;
    
    while ((type = va_arg(ap, ZVM_MessageArgumentType))
           != ZVM_MESSAGE_ARGUMENT_END) {
        arg[index].type = type;
        arg[index].name = va_arg(ap, char*);
        switch (type) {
        case ZVM_INT_MESSAGE_ARGUMENT:
            arg[index].u.int_val = va_arg(ap, int);
            break;
        case ZVM_DOUBLE_MESSAGE_ARGUMENT:
            arg[index].u.double_val = va_arg(ap, double);
            break;
        case ZVM_STRING_MESSAGE_ARGUMENT:
            arg[index].u.string_val = va_arg(ap, char*);
            break;
        case ZVM_POINTER_MESSAGE_ARGUMENT:
            arg[index].u.pointer_val = va_arg(ap, void*);
            break;
        case ZVM_CHARACTER_MESSAGE_ARGUMENT:
            arg[index].u.character_val = va_arg(ap, int);
            break;
        case ZVM_MESSAGE_ARGUMENT_END:
            assert(0);
            break;
        default:
            assert(0);
        }
        index++;
        assert(index < MESSAGE_ARGUMENT_MAX);
    }
}

static void
search_argument(MessageArgument *arg_list,
                char *arg_name, MessageArgument *arg)
{
    int i;

    for (i = 0; arg_list[i].type != ZVM_MESSAGE_ARGUMENT_END; i++) {
        if (!strcmp(arg_list[i].name, arg_name)) {
            *arg = arg_list[i];
            return;
        }
    }
    assert(0);
}

static void
format_message(ZVM_ErrorDefinition *format, VString *v,
               va_list ap)
{
    int         i;
    char        buf[LINE_BUF_SIZE];
    ZVM_Char    wc_buf[LINE_BUF_SIZE];
    int         arg_name_index;
    char        arg_name[LINE_BUF_SIZE];
    MessageArgument     arg[MESSAGE_ARGUMENT_MAX];
    MessageArgument     cur_arg;
    ZVM_Char    *wc_format;

    create_message_argument(arg, ap);

    wc_format = zvm_mbstowcs_alloc(NULL, format->format);
    DBG_assert(wc_format != NULL, ("wc_format is null.\n"));
    
    for (i = 0; wc_format[i] != L'\0'; i++) {
        if (wc_format[i] != L'$') {
            zvm_vstr_append_character(v, wc_format[i]);
            continue;
        }
        assert(wc_format[i+1] == L'(');
        i += 2;
        for (arg_name_index = 0; wc_format[i] != L')';
             arg_name_index++, i++) {
            arg_name[arg_name_index] = zvm_wctochar(wc_format[i]);
        }
        arg_name[arg_name_index] = '\0';
        assert(wc_format[i] == L')');

        search_argument(arg, arg_name, &cur_arg);
        switch (cur_arg.type) {
        case ZVM_INT_MESSAGE_ARGUMENT:
            sprintf(buf, "%d", cur_arg.u.int_val);
            zvm_mbstowcs(buf, wc_buf);
            zvm_vstr_append_string(v, wc_buf);
            break;
        case ZVM_DOUBLE_MESSAGE_ARGUMENT:
            sprintf(buf, "%f", cur_arg.u.double_val);
            zvm_mbstowcs(buf, wc_buf);
            zvm_vstr_append_string(v, wc_buf);
            break;
        case ZVM_STRING_MESSAGE_ARGUMENT:
            zvm_mbstowcs(cur_arg.u.string_val, wc_buf);
            zvm_vstr_append_string(v, wc_buf);
            break;
        case ZVM_POINTER_MESSAGE_ARGUMENT:
            sprintf(buf, "%p", cur_arg.u.pointer_val);
            zvm_mbstowcs(buf, wc_buf);
            zvm_vstr_append_string(v, wc_buf);
            break;
        case ZVM_CHARACTER_MESSAGE_ARGUMENT:
            sprintf(buf, "%c", cur_arg.u.character_val);
            zvm_mbstowcs(buf, wc_buf);
            zvm_vstr_append_string(v, wc_buf);
            break;
        case ZVM_MESSAGE_ARGUMENT_END:
            assert(0);
            break;
        default:
            assert(0);
        }
    }
    MEM_free(wc_format);
}

static void
self_check()
{
    if (strcmp(zvm_error_message_format[0].format, "dummy") != 0) {
        DBG_panic(("runtime error message format error.\n"));
    }
    if (strcmp(zvm_error_message_format
               [RUNTIME_ERROR_COUNT_PLUS_1].format,
               "dummy") != 0) {
        DBG_panic(("runtime error message format error. "
                   "RUNTIME_ERROR_COUNT_PLUS_1..%d\n",
                   RUNTIME_ERROR_COUNT_PLUS_1));
    }
}

int
zvm_conv_pc_to_line_number(ZVM_Executable *exe, Function *func, int pc)
{
    ZVM_LineNumber *line_number;
    int line_number_size;
    int i;
    int ret;

    if (func) {
        line_number
            = exe->function[func->u.zero_f.index].code_block.line_number;
        line_number_size
            = exe->function[func->u.zero_f.index]
            .code_block.line_number_size;
    } else {
        line_number = exe->top_level.line_number;
        line_number_size = exe->top_level.line_number_size;
    }

    for (i = 0; i < line_number_size; i++) {
        if (pc >= line_number[i].start_pc
            && pc < line_number[i].start_pc + line_number[i].pc_count) {
            ret = line_number[i].line_number;
        }
    }

    return ret;
}

static void
error_v(ZVM_Executable *exe, Function *func, int pc, RuntimeError id,
        va_list ap)
{
    VString     message;
    int         line_number;

    zvm_vstr_clear(&message);
    format_message(&zvm_error_message_format[id],
                   &message, ap);

    if (pc != NO_LINE_NUMBER_PC) {
        line_number = zvm_conv_pc_to_line_number(exe, func, pc);
        fprintf(stderr, "%s:%d:", exe->path, line_number);
    }
    zvm_print_wcs_ln(stderr, message.string);
}

void
zvm_error_i(ZVM_Executable *exe, Function *func, int pc,
            RuntimeError id, ...)
{
    va_list     ap;

    self_check();
    va_start(ap, id);
    error_v(exe, func, pc, id, ap);
    va_end(ap);

    exit(1);
}

void
zvm_error_n(ZVM_VirtualMachine *zvm, RuntimeError id, ...)
{
    va_list     ap;

    self_check();
    va_start(ap, id);
    error_v(zvm->current_executable->executable,
            zvm->current_function, zvm->pc, id, ap);
    va_end(ap);

    exit(1);
}

void
zvm_format_message(ZVM_ErrorDefinition *error_definition,
                   int id, VString *message, va_list ap)
{
    zvm_vstr_clear(message);
    format_message(&error_definition[id],
                   message, ap);
}
