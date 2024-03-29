#include "MEM.h"
#include "DBG.h"
#include "ZVM_code.h"

static void dispose_local_variable(int local_variable_count,
                                   ZVM_LocalVariable *local_variable_list);

static void
dispose_type_derive(ZVM_TypeSpecifier *type)
{
    int i;

    for (i = 0; i < type->derive_count; i++) {
        switch (type->derive[i].tag) {
        case ZVM_FUNCTION_DERIVE:
            dispose_local_variable(type->derive[i].u
                                   .function_d.parameter_count,
                                   type->derive[i].u
                                   .function_d.parameter);
            break;
        case ZVM_ARRAY_DERIVE:
            break;
        default:
            DBG_assert(0, ("derive->tag..%d\n", type->derive[i].tag));
        }
    }
    MEM_free(type->derive);
}

static void
dispose_type_specifier(ZVM_TypeSpecifier *type)
{
    dispose_type_derive(type);
    MEM_free(type);
}

static void
dispose_local_variable(int local_variable_count,
                       ZVM_LocalVariable *local_variable)
{
    int i;

    for (i = 0; i < local_variable_count; i++) {
        MEM_free(local_variable[i].name);
        dispose_type_specifier(local_variable[i].type);
    }
    MEM_free(local_variable);
}

static void
dispose_class(ZVM_Class *cd)
{
    int i;

    for (i = 0; i < cd->field_count; i++) {
        MEM_free(cd->field[i].name);
        dispose_type_specifier(cd->field[i].type);
    }
    MEM_free(cd->field);

    for (i = 0; i < cd->method_count; i++) {
        MEM_free(cd->method[i].name);
    }
    MEM_free(cd->method);

    if (cd->super_class) {
        MEM_free(cd->super_class->package_name);
        MEM_free(cd->super_class->name);
        MEM_free(cd->super_class);
    }

    for (i = 0; i < cd->interface_count; i++) {
        MEM_free(cd->interface[i].package_name);
        MEM_free(cd->interface[i].name);
    }
    MEM_free(cd->interface);

    MEM_free(cd->package_name);
    MEM_free(cd->name);
}

static void
dispose_enum(ZVM_Enum *enum_type)
{
    int i;

    MEM_free(enum_type->name);

    for (i = 0; i < enum_type->enumerator_count; i++) {
        MEM_free(enum_type->enumerator[i]);
    }
    MEM_free(enum_type->enumerator);
}

static void
dispose_code_block(ZVM_CodeBlock *cb)
{
    int try_idx;

    MEM_free(cb->code);
    MEM_free(cb->line_number);

    for (try_idx = 0; try_idx < cb->try_size; try_idx++) {
        MEM_free(cb->try[try_idx].catch_clause);
    }
    MEM_free(cb->try);
}

void
zvm_dispose_executable(ZVM_Executable *exe)
{
    int i;

    MEM_free(exe->package_name);
    MEM_free(exe->path);

    for (i = 0; i < exe->constant_pool_count; i++) {
        if (exe->constant_pool[i].tag == ZVM_CONSTANT_STRING) {
            MEM_free(exe->constant_pool[i].u.c_string);
        }
    }
    MEM_free(exe->constant_pool);
    
    for (i = 0; i < exe->global_variable_count; i++) {
        MEM_free(exe->global_variable[i].name);
        dispose_type_specifier(exe->global_variable[i].type);
    }
    MEM_free(exe->global_variable);

    for (i = 0; i < exe->function_count; i++) {
        MEM_free(exe->function[i].name);
        MEM_free(exe->function[i].package_name);
        dispose_type_specifier(exe->function[i].type);
        dispose_local_variable(exe->function[i].parameter_count,
                               exe->function[i].parameter);
        if (exe->function[i].is_implemented) {
            dispose_local_variable(exe->function[i].local_variable_count,
                                   exe->function[i].local_variable);
            dispose_code_block(&exe->function[i].code_block);
        }
    }
    MEM_free(exe->function);

    for (i = 0; i < exe->type_specifier_count; i++) {
        dispose_type_derive(&exe->type_specifier[i]);
    }
    MEM_free(exe->type_specifier);

    for (i = 0; i < exe->class_count; i++) {
        dispose_class(&exe->class_definition[i]);
    }
    MEM_free(exe->class_definition);

    for (i = 0; i < exe->enum_count; i++) {
        dispose_enum(&exe->enum_definition[i]);
    }
    MEM_free(exe->enum_definition);

    dispose_code_block(&exe->top_level);
    MEM_free(exe);
}
