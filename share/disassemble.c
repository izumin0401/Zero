#include <stdio.h>
#include <string.h>
#include "DBG.h"
#include "share.h"

extern OpcodeInfo zvm_opcode_info[];

static void
dump_constant_pool(int constant_pool_count, ZVM_ConstantPool *constant_pool)
{
    int i;

    printf("** constant pool section ***********************************\n");
    for (i = 0; i < constant_pool_count; i++) {
        printf("%05d:", i);
        switch (constant_pool[i].tag) {
        case ZVM_CONSTANT_INT:
            printf("int %d\n", constant_pool[i].u.c_int);
            break;
        case ZVM_CONSTANT_DOUBLE:
            printf("double %f\n", constant_pool[i].u.c_double);
            break;
        case ZVM_CONSTANT_STRING:
            printf("string ");
            zvm_print_wcs_ln(stdout, constant_pool[i].u.c_string);
            break;
        default:
            DBG_assert(0, ("tag..%d\n", constant_pool[i].tag));
        }
    }
}

static void dump_type(ZVM_Executable *exe, ZVM_TypeSpecifier *type);

static void
dump_parameter_list(ZVM_Executable *exe,
                    int parameter_count, ZVM_LocalVariable *parameter_list)
{
    int i;

    printf("(");
    for (i = 0; i < parameter_count; i++) {
        dump_type(exe, parameter_list[i].type);
        printf(" %s", parameter_list[i].name);
        if (i < parameter_count-1) {
            printf(", ");
        }
    }
    printf(")");
}

static void
dump_type(ZVM_Executable *exe, ZVM_TypeSpecifier *type)
{
    int i;

    switch (type->basic_type) {
    case ZVM_VOID_TYPE:
        printf("void ");
        break;
    case ZVM_BOOLEAN_TYPE:
        printf("boolean ");
        break;
    case ZVM_INT_TYPE:
        printf("int ");
        break;
    case ZVM_DOUBLE_TYPE:
        printf("double ");
        break;
    case ZVM_STRING_TYPE:
        printf("string ");
        break;
    case ZVM_NATIVE_POINTER_TYPE:
        printf("native_pointer ");
        break;
    case ZVM_CLASS_TYPE:
        printf("<%s> ", exe->class_definition[type->u.class_t.index].name);
        break;
    case ZVM_DELEGATE_TYPE:
        printf("delegate ");
        break;
    case ZVM_ENUM_TYPE:
        printf("<%s> ", exe->enum_definition[type->u.enum_t.index].name);
        break;
    case ZVM_NULL_TYPE:
        printf("null ");
        break;
    case ZVM_BASE_TYPE: /* FALLTHRU */
    case ZVM_UNSPECIFIED_IDENTIFIER_TYPE:
    default:
        DBG_assert(0, ("basic_type..%d\n", type->basic_type));
    }

    for (i = 0; i < type->derive_count; i++) {
        switch (type->derive[i].tag) {
        case ZVM_FUNCTION_DERIVE:
            dump_parameter_list(exe,
                                type->derive[i].u.function_d.parameter_count,
                                type->derive[i].u.function_d.parameter);
            break;
        case ZVM_ARRAY_DERIVE:
            printf("[]");
            break;
        default:
            DBG_assert(0, ("derive_tag..%d\n", type->derive->tag));
        }
    }
}

static void
dump_variable(ZVM_Executable *exe, int global_variable_count,
              ZVM_Variable *global_variable)
{
    int i;

    printf("** global variable section *********************************\n");
    for (i = 0; i < global_variable_count; i++) {
        printf("%5d:", i);
        dump_type(exe, global_variable[i].type);
        printf(" %s\n", global_variable[i].name);
    }
}

int
zvm_dump_instruction(FILE *fp, ZVM_Byte *code, int index)
{
    OpcodeInfo *info;
    int value;
    int i;

    info = &zvm_opcode_info[code[index]];
    fprintf(fp, "%4d %s ", index, info->mnemonic);
    for (i = 0; info->parameter[i] != '\0'; i++) {
        switch (info->parameter[i]) {
        case 'b':
            value = code[index+1];
            fprintf(fp, " %d", value);
            index++;
            break;
        case 's': /* FALLTHRU */
        case 'p':
            value = (code[index+1] << 8) + code[index+2];
            fprintf(fp, " %d", value);
            index += 2;
            break;
        default:
            DBG_assert(0, ("param..%s, i..%d", info->parameter, i));
        }
    }
    index++;

    return index;
}

static void
dump_opcode(int code_size, ZVM_Byte *code)
{
    int index;

    for (index = 0; index < code_size; ) {
        index = zvm_dump_instruction(stdout, code, index);
        printf("\n");
    }
}

static void
dump_line_number(int line_number_size, ZVM_LineNumber *line_number)
{
    int i;

    printf("*** line number ***\n");
    for (i = 0; i < line_number_size; i++) {
        printf("%5d: from %5d size %5d\n",
               line_number[i].line_number,
               line_number[i].start_pc,
               line_number[i].pc_count);
    }
}

static void
dump_function(ZVM_Executable *exe, int function_count, ZVM_Function *function)
{
    int i;

    printf("** function section ****************************************\n");
    for (i = 0; i < function_count; i++) {
        printf("*** [%d] %s ***\n", i, function[i].name);

        dump_type(exe, function[i].type);
        printf(" %s ", function[i].name);
        dump_parameter_list(exe,
                            function[i].parameter_count,
                            function[i].parameter);
        printf("\n");
        if (function[i].is_implemented) {
            if (function[i].code_block.code_size > 0) {
                dump_opcode(function[i].code_block.code_size,
                            function[i].code_block.code);
                dump_line_number(function[i].code_block.line_number_size,
                                 function[i].code_block.line_number);
            }
        }
        printf("*** end of %s ***\n", function[i].name);
    }
}

static void
dump_types(ZVM_Executable *exe, int type_count, ZVM_TypeSpecifier *types)
{
    int i;

    printf("** type section ********************************************\n");
    for (i = 0; i < type_count; i++) {
        printf("%5d:", i);
        dump_type(exe, &types[i]);
        printf("\n");
    }
}

void
zvm_disassemble(ZVM_Executable *exe)
{
    dump_constant_pool(exe->constant_pool_count, exe->constant_pool);
    dump_variable(exe, exe->global_variable_count, exe->global_variable);
    dump_function(exe, exe->function_count, exe->function);
    dump_types(exe, exe->type_specifier_count, exe->type_specifier);
    printf("** toplevel ********************\n");
    dump_opcode(exe->top_level.code_size, exe->top_level.code);
    dump_line_number(exe->top_level.line_number_size,
                     exe->top_level.line_number);
}
