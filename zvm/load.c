#include <stdio.h>
#include <string.h>
#include "ZEC.h"
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

static void
implement_zero_function(ZVM_VirtualMachine *zvm, int dest_idx,
                          ExecutableEntry *ee, int src_idx)
{
    zvm->function[dest_idx]->u.zero_f.executable
        = ee;
    zvm->function[dest_idx]->u.zero_f.index = src_idx;
}

static void
add_functions(ZVM_VirtualMachine *zvm, ExecutableEntry *ee)
{
    int src_idx;
    int dest_idx;
    int add_func_count = 0;
    ZVM_Boolean *new_func_flags;

    new_func_flags = MEM_malloc(sizeof(ZVM_Boolean)
                                * ee->executable->function_count);

    for (src_idx = 0; src_idx < ee->executable->function_count; src_idx++) {
        for (dest_idx = 0; dest_idx < zvm->function_count; dest_idx++) {
            if (!strcmp(zvm->function[dest_idx]->name,
                        ee->executable->function[src_idx].name)
                && zvm_compare_package_name(zvm->function[dest_idx]
                                            ->package_name,
                                            ee->executable->function
                                            [src_idx].package_name)) {
                if (ee->executable->function[src_idx].is_implemented
                    && zvm->function[dest_idx]->is_implemented) {
                    char *package_name;

                    if (zvm->function[dest_idx]->package_name) {
                        package_name = zvm->function[dest_idx]->package_name;
                    } else {
                        package_name = "";
                    }
                    zvm_error_i(NULL, NULL, NO_LINE_NUMBER_PC,
                                FUNCTION_MULTIPLE_DEFINE_ERR,
                                ZVM_STRING_MESSAGE_ARGUMENT, "package",
                                package_name,
                                ZVM_STRING_MESSAGE_ARGUMENT, "name",
                                zvm->function[dest_idx]->name,
                                ZVM_MESSAGE_ARGUMENT_END);
                }
                new_func_flags[src_idx] = ZVM_FALSE;
                if (ee->executable->function[src_idx].is_implemented) {
                    implement_zero_function(zvm, dest_idx, ee, src_idx);
                }
                break;
            }
        }
        if (dest_idx == zvm->function_count) {
            new_func_flags[src_idx] = ZVM_TRUE;
            add_func_count++;
        }
    }
    zvm->function
        = MEM_realloc(zvm->function,
                      sizeof(Function*)
                      * (zvm->function_count + add_func_count));

    for (src_idx = 0, dest_idx = zvm->function_count;
         src_idx < ee->executable->function_count; src_idx++) {
        if (!new_func_flags[src_idx])
            continue;

        zvm->function[dest_idx] = MEM_malloc(sizeof(Function));
        if (ee->executable->function[src_idx].package_name) {
            zvm->function[dest_idx]->package_name
                = MEM_strdup(ee->executable->function[src_idx].package_name);
        } else {
            zvm->function[dest_idx]->package_name = NULL;
        }
        zvm->function[dest_idx]->name
            = MEM_strdup(ee->executable->function[src_idx].name);
        zvm->function[dest_idx]->kind = ZERO_FUNCTION;
        zvm->function[dest_idx]->is_implemented
            = ee->executable->function[src_idx].is_implemented;
        if (ee->executable->function[src_idx].is_implemented) {
            implement_zero_function(zvm, dest_idx, ee, src_idx);
        }
        dest_idx++;
    }
    zvm->function_count += add_func_count;
    MEM_free(new_func_flags);
}


int
zvm_search_class(ZVM_VirtualMachine *zvm, char *package_name, char *name)
{
    int i;

    for (i = 0; i < zvm->class_count; i++) {
        if (zvm_compare_package_name(zvm->class[i]->package_name,
                                     package_name)
            && !strcmp(zvm->class[i]->name, name)) {
            return i;
        }
    }
    zvm_error_i(NULL, NULL, NO_LINE_NUMBER_PC, CLASS_NOT_FOUND_ERR,
                ZVM_STRING_MESSAGE_ARGUMENT, "name", name,
                ZVM_MESSAGE_ARGUMENT_END);
    return 0; /* make compiler happy */
}

int
zvm_search_function(ZVM_VirtualMachine *zvm, char *package_name, char *name)
{
    int i;

    for (i = 0; i < zvm->function_count; i++) {
        if (zvm_compare_package_name(zvm->function[i]->package_name,
                                     package_name)
            && !strcmp(zvm->function[i]->name, name)) {
            return i;
        }
    }
    return FUNCTION_NOT_FOUND;
}

static void
convert_code(ZVM_VirtualMachine *zvm, ZVM_Executable *exe,
             ZVM_Byte *code, int code_size, ZVM_Function *func)
{
    int i;
    int j;
    OpcodeInfo *info;
    int src_idx;
    unsigned int dest_idx;

    for (i = 0; i < code_size; i++) {
        if (code[i] == ZVM_PUSH_STACK_INT
            || code[i] == ZVM_PUSH_STACK_DOUBLE
            || code[i] == ZVM_PUSH_STACK_OBJECT
            || code[i] == ZVM_POP_STACK_INT
            || code[i] == ZVM_POP_STACK_DOUBLE
            || code[i] == ZVM_POP_STACK_OBJECT) {
            int parameter_count;

            DBG_assert(func != NULL, ("func == NULL!\n"));
            
            if (func->is_method) {
                parameter_count = func->parameter_count + 1; /* for this */
            } else{
                parameter_count = func->parameter_count;
            }

            src_idx = GET_2BYTE_INT(&code[i+1]);
            if (src_idx >= parameter_count) {
                dest_idx = src_idx + CALL_INFO_ALIGN_SIZE;
            } else {
                dest_idx = src_idx;
            }
            SET_2BYTE_INT(&code[i+1], dest_idx);
        }
        info = &zvm_opcode_info[code[i]];
        for (j = 0; info->parameter[j] != '\0'; j++) {
            switch (info->parameter[j]) {
            case 'b':
                i++;
                break;
            case 's': /* FALLTHRU */
            case 'p':
                i += 2;
                break;
            default:
                DBG_assert(0, ("param..%s, j..%d", info->parameter, j));
            }
        }
    }
}

static void
add_reference_table(ZVM_VirtualMachine *zvm,
                    ExecutableEntry *entry, ZVM_Executable *exe)
{
    int i;

    entry->function_table = MEM_malloc(sizeof(int) * exe->function_count);
    for (i = 0; i < exe->function_count; i++) {
        entry->function_table[i]
            = zvm_search_function(zvm, exe->function[i].package_name,
                                  exe->function[i].name);
    }

    entry->class_table = MEM_malloc(sizeof(int) * exe->class_count);
    for (i = 0; i < exe->class_count; i++) {
        entry->class_table[i]
            = zvm_search_class(zvm, exe->class_definition[i].package_name,
                               exe->class_definition[i].name);
    }
}

static void
add_static_variables(ExecutableEntry *entry, ZVM_Executable *exe)
{
    int i;

    entry->static_v.variable
        = MEM_malloc(sizeof(ZVM_Value) * exe->global_variable_count);
    entry->static_v.variable_count = exe->global_variable_count;

    for (i = 0; i < exe->global_variable_count; i++) {
        if (exe->global_variable[i].type->basic_type == ZVM_STRING_TYPE) {
            entry->static_v.variable[i].object = zvm_null_object_ref;
        }
    }
    for (i = 0; i < exe->global_variable_count; i++) {
        zvm_initialize_value(exe->global_variable[i].type,
                             &entry->static_v.variable[i]);
    }
}

static ZVM_Class *
search_class_from_executable(ZVM_Executable *exe, char *name)
{
    int i;

    for (i = 0; i < exe->class_count; i++) {
        if (!strcmp(exe->class_definition[i].name, name)) {
            return &exe->class_definition[i];
        }
    }
    DBG_panic(("class %s not found.", name));

    return NULL; /* make compiler happy */
}

static int
set_field_types(ZVM_Executable *exe, ZVM_Class *pos,
                ZVM_TypeSpecifier **field_type, int index)
{
    ZVM_Class *next;
    int i;

    if (pos->super_class) {
        next = search_class_from_executable(exe, pos->super_class->name);
        index = set_field_types(exe, next, field_type, index);
    }
    for (i = 0; i < pos->field_count; i++) {
        field_type[index] = pos->field[i].type;
        index++;
    }

    return index;
}

static void
add_fields(ZVM_Executable *exe, ZVM_Class *src, ExecClass *dest)
{
    int field_count = 0;
    ZVM_Class *pos;

    for (pos = src; ; ) {
        field_count += pos->field_count;
        if (pos->super_class == NULL)
            break;
        pos = search_class_from_executable(exe, pos->super_class->name);
    }
    dest->field_count = field_count;
    dest->field_type = MEM_malloc(sizeof(ZVM_TypeSpecifier*) * field_count);
    set_field_types(exe, src, dest->field_type, 0);
}

static void
set_v_table(ZVM_VirtualMachine *zvm, ZVM_Class *class_p,
            ZVM_Method *src, VTableItem *dest, ZVM_Boolean set_name)
{
    char *func_name;
    int  func_idx;

    if (set_name) {
        dest->name = MEM_strdup(src->name);
    }
    func_name = zvm_create_method_function_name(class_p->name, src->name);
    func_idx = zvm_search_function(zvm, class_p->package_name, func_name);

    if (func_idx == FUNCTION_NOT_FOUND && !src->is_abstract) {
        zvm_error_i(NULL, NULL, NO_LINE_NUMBER_PC, FUNCTION_NOT_FOUND_ERR,
                    ZVM_STRING_MESSAGE_ARGUMENT, "name", func_name,
                    ZVM_MESSAGE_ARGUMENT_END);
    }
    MEM_free(func_name);
    dest->index = func_idx;
}

static int
add_method(ZVM_VirtualMachine *zvm, ZVM_Executable *exe, ZVM_Class *pos,
           ZVM_VTable *v_table)
{
    ZVM_Class   *next;
    int         i;
    int         j;
    int         super_method_count = 0;
    int         method_count;

    if (pos->super_class) {
        next = search_class_from_executable(exe, pos->super_class->name);
        super_method_count = add_method(zvm, exe, next, v_table);
    }

    method_count = super_method_count;
    for (i = 0; i < pos->method_count; i++) {
        for (j = 0; j < super_method_count; j++) {
            if (!strcmp(pos->method[i].name, v_table->table[j].name)) {
                set_v_table(zvm, pos, &pos->method[i], &v_table->table[j],
                            ZVM_FALSE);
                break;
            }
        }
        /* if pos->method[i].is_override == true,
         * this method implements interface method.
         */
        if (j == super_method_count && !pos->method[i].is_override) {
            v_table->table
                = MEM_realloc(v_table->table,
                              sizeof(VTableItem) * (method_count + 1));
            set_v_table(zvm, pos, &pos->method[i],
                        &v_table->table[method_count], ZVM_TRUE);
            method_count++;
            v_table->table_size = method_count;
        }
    }

    return method_count;
}

static ZVM_VTable *
alloc_v_table(ExecClass *exec_class)
{
    ZVM_VTable *v_table;

    v_table = MEM_malloc(sizeof(ZVM_VTable));
    v_table->exec_class = exec_class;
    v_table->table = NULL;

    return v_table;
}

static void
add_methods(ZVM_VirtualMachine *zvm, ZVM_Executable *exe,
            ZVM_Class *src, ExecClass *dest)
{
    int         method_count;
    ZVM_VTable  *v_table;
    int         i;
    ZVM_Class   *interface;
    int         method_idx;

    v_table = alloc_v_table(dest);
    method_count = add_method(zvm, exe, src, v_table);
    dest->class_table = v_table;
    dest->interface_count = src->interface_count;
    dest->interface_v_table
        = MEM_malloc(sizeof(ZVM_VTable*) * src->interface_count);

    for (i = 0; i < src->interface_count; i++) {
        dest->interface_v_table[i] = alloc_v_table(dest);
        interface = search_class_from_executable(exe,
                                                 src->interface[i].name);
        dest->interface_v_table[i]->table
            = MEM_malloc(sizeof(VTableItem) * interface->method_count);
        dest->interface_v_table[i]->table_size = interface->method_count;
        for (method_idx = 0; method_idx < interface->method_count;
             method_idx++) {
            set_v_table(zvm, src, &interface->method[method_idx],
                        &dest->interface_v_table[i]->table[method_idx],
                        ZVM_TRUE);
        }
    }
}

static void
add_class(ZVM_VirtualMachine *zvm, ZVM_Executable *exe,
          ZVM_Class *src, ExecClass *dest)
{
    
    add_fields(exe, src, dest);
    add_methods(zvm, exe, src, dest);
}

static void
set_super_class(ZVM_VirtualMachine *zvm, ZVM_Executable *exe,
                int old_class_count)
{
    int class_idx;
    ZVM_Class *zvm_class;
    int super_class_index;
    int if_idx;
    int interface_index;

    for (class_idx = old_class_count; class_idx < zvm->class_count;
         class_idx++) {
        zvm_class = search_class_from_executable(exe,
                                                 zvm->class[class_idx]->name);
        if (zvm_class->super_class == NULL) {
            zvm->class[class_idx]->super_class = NULL;
        } else {
            super_class_index
                = zvm_search_class(zvm,
                                   zvm_class->super_class
                                   ->package_name,
                                   zvm_class->super_class->name);
            zvm->class[class_idx]->super_class = zvm->class[super_class_index];
        }
        zvm->class[class_idx]->interface
            = MEM_malloc(sizeof(ExecClass*) * zvm_class->interface_count);
        for (if_idx = 0; if_idx < zvm_class->interface_count; if_idx++) {
            interface_index
                = zvm_search_class(zvm,
                                   zvm_class->interface[if_idx].package_name,
                                   zvm_class->interface[if_idx].name);
            zvm->class[class_idx]->interface[if_idx]
                = zvm->class[interface_index];
        }
    }
}

static void
add_classes(ZVM_VirtualMachine *zvm, ZVM_Executable *exe)
{
    int src_idx;
    int dest_idx;
    int add_class_count = 0;
    ZVM_Boolean *new_class_flags;
    int old_class_count;

    new_class_flags = MEM_malloc(sizeof(ZVM_Boolean)
                                 * exe->class_count);

    for (src_idx = 0; src_idx < exe->class_count; src_idx++) {
        for (dest_idx = 0; dest_idx < zvm->class_count; dest_idx++) {
            if (!strcmp(zvm->class[dest_idx]->name,
                        exe->class_definition[src_idx].name)
                && zvm_compare_package_name(zvm->class[dest_idx]
                                            ->package_name,
                                            exe->class_definition[src_idx]
                                            .package_name)) {
                if (exe->class_definition[src_idx].is_implemented
                    && zvm->class[dest_idx]->is_implemented) {
                    char *package_name;

                    if (zvm->class[dest_idx]->package_name) {
                        package_name = zvm->class[dest_idx]->package_name;
                    } else {
                        package_name = "";
                    }
                    zvm_error_i(NULL, NULL, NO_LINE_NUMBER_PC,
                                CLASS_MULTIPLE_DEFINE_ERR,
                                ZVM_STRING_MESSAGE_ARGUMENT, "package",
                                package_name,
                                ZVM_STRING_MESSAGE_ARGUMENT, "name",
                                zvm->class[dest_idx]->name,
                                ZVM_MESSAGE_ARGUMENT_END);
                }
                new_class_flags[src_idx] = ZVM_FALSE;
                if (exe->class_definition[src_idx].is_implemented) {
                    add_class(zvm, exe, &exe->class_definition[src_idx],
                              zvm->class[dest_idx]);
                }
                break;
            }
        }
        if (dest_idx == zvm->class_count) {
            new_class_flags[src_idx] = ZVM_TRUE;
            add_class_count++;
        }
    }
    zvm->class
        = MEM_realloc(zvm->class, sizeof(ExecClass*)
                      * (zvm->class_count + add_class_count));

    for (src_idx = 0, dest_idx = zvm->class_count;
         src_idx < exe->class_count; src_idx++) {
        if (!new_class_flags[src_idx])
            continue;

        zvm->class[dest_idx] = MEM_malloc(sizeof(ExecClass));
        zvm->class[dest_idx]->zvm_class = &exe->class_definition[src_idx];
        if (exe->class_definition[src_idx].package_name) {
            zvm->class[dest_idx]->package_name
                = MEM_strdup(exe->class_definition[src_idx].package_name);
        } else {
            zvm->class[dest_idx]->package_name = NULL;
        }
        zvm->class[dest_idx]->name
            = MEM_strdup(exe->class_definition[src_idx].name);
        zvm->class[dest_idx]->is_implemented
            = exe->class_definition[src_idx].is_implemented;
        zvm->class[dest_idx]->class_index = dest_idx;
        if (exe->class_definition[src_idx].is_implemented) {
            add_class(zvm, exe, &exe->class_definition[src_idx],
                      zvm->class[dest_idx]);
        }
        dest_idx++;
    }
    old_class_count = zvm->class_count;
    zvm->class_count += add_class_count;

    set_super_class(zvm, exe, old_class_count);

    MEM_free(new_class_flags);
}

static void
add_executable_to_zvm(ZVM_VirtualMachine *zvm, ZVM_Executable *executable,
                      ZVM_Boolean is_top_level)
{
    int i;
    ExecutableEntry *ee_pos;
    ExecutableEntry *new_entry;
    
    new_entry = MEM_malloc(sizeof(ExecutableEntry));
    new_entry->executable = executable;
    new_entry->next = NULL;

    if (zvm->executable_entry == NULL) {
        zvm->executable_entry = new_entry;
    } else {
        for (ee_pos = zvm->executable_entry; ee_pos->next;
             ee_pos = ee_pos->next)
            ;
        ee_pos->next = new_entry;
    }
    
    add_functions(zvm, new_entry);
    add_classes(zvm, executable);

    convert_code(zvm, executable,
                 executable->top_level.code, executable->top_level.code_size,
                 NULL);

    for (i = 0; i < executable->function_count; i++) {
        if (executable->function[i].is_implemented) {
            convert_code(zvm, executable,
                         executable->function[i].code_block.code,
                         executable->function[i].code_block.code_size,
                         &executable->function[i]);
        }
    }
    add_reference_table(zvm, new_entry, executable);

    add_static_variables(new_entry, executable);

    if (is_top_level) {
        zvm->top_level = new_entry;
    }
}

void
ZVM_set_executable(ZVM_VirtualMachine *zvm, ZVM_ExecutableList *list)
{
    ZVM_ExecutableItem *pos;
    int old_class_count;

    zvm->executable_list = list;

    old_class_count = zvm->class_count;
    for (pos = list->list; pos; pos = pos->next) {
        add_executable_to_zvm(zvm, pos->executable,
                              (pos->executable == list->top_level));
    }
}

static VTableItem st_array_method_v_table[] = {
    {ARRAY_PREFIX ARRAY_METHOD_SIZE, FUNCTION_NOT_FOUND},
    {ARRAY_PREFIX ARRAY_METHOD_RESIZE, FUNCTION_NOT_FOUND},
    {ARRAY_PREFIX ARRAY_METHOD_INSERT, FUNCTION_NOT_FOUND},
    {ARRAY_PREFIX ARRAY_METHOD_REMOVE, FUNCTION_NOT_FOUND},
    {ARRAY_PREFIX ARRAY_METHOD_ADD, FUNCTION_NOT_FOUND}
};

static VTableItem st_string_method_v_table[] = {
    {STRING_PREFIX STRING_METHOD_LENGTH, FUNCTION_NOT_FOUND},
    {STRING_PREFIX STRING_METHOD_SUBSTR, FUNCTION_NOT_FOUND},
};

static void
set_built_in_methods(ZVM_VirtualMachine *zvm)
{
    ZVM_VTable *array_v_table;
    ZVM_VTable *string_v_table;
    int i;

    array_v_table = alloc_v_table(NULL);
    array_v_table->table_size = ARRAY_SIZE(st_array_method_v_table);
    array_v_table->table = MEM_malloc(sizeof(VTableItem)
                                      * array_v_table->table_size);
    for (i = 0; i < array_v_table->table_size; i++) {
        array_v_table->table[i] = st_array_method_v_table[i];
        array_v_table->table[i].index
            = zvm_search_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                                  array_v_table->table[i].name);
    }
    zvm->array_v_table = array_v_table;

    string_v_table = alloc_v_table(NULL);
    string_v_table->table_size = ARRAY_SIZE(st_string_method_v_table);
    string_v_table->table = MEM_malloc(sizeof(VTableItem)
                                      * string_v_table->table_size);
    for (i = 0; i < string_v_table->table_size; i++) {
        string_v_table->table[i] = st_string_method_v_table[i];
        string_v_table->table[i].index
            = zvm_search_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                                  string_v_table->table[i].name);
    }
    zvm->string_v_table = string_v_table;
}

ZVM_VirtualMachine *
ZVM_create_virtual_machine(void)
{
    ZVM_VirtualMachine *zvm;

    zvm = MEM_malloc(sizeof(ZVM_VirtualMachine));
    zvm->stack.alloc_size = STACK_ALLOC_SIZE;
    zvm->stack.stack = MEM_malloc(sizeof(ZVM_Value) * STACK_ALLOC_SIZE);
    zvm->stack.pointer_flags
        = MEM_malloc(sizeof(ZVM_Boolean) * STACK_ALLOC_SIZE);
    zvm->stack.stack_pointer = 0;
    zvm->heap.current_heap_size = 0;
    zvm->heap.header = NULL;
    zvm->heap.current_threshold = HEAP_THRESHOLD_SIZE;
    zvm->current_executable = NULL;
    zvm->current_function = NULL;
    zvm->current_exception = zvm_null_object_ref;
    zvm->function = NULL;
    zvm->function_count = 0;
    zvm->class = NULL;
    zvm->class_count = 0;
    zvm->executable_list = NULL;
    zvm->executable_entry = NULL;
    zvm->top_level = NULL;
    zvm->current_context = NULL;

    zvm_add_native_functions(zvm);

    set_built_in_methods(zvm);

    return zvm;
}

void
ZVM_add_native_function(ZVM_VirtualMachine *zvm,
                        char *package_name, char *func_name,
                        ZVM_NativeFunctionProc *proc, int arg_count,
                        ZVM_Boolean is_method, ZVM_Boolean return_pointer)
{
    zvm->function
        = MEM_realloc(zvm->function,
                      sizeof(Function*) * (zvm->function_count + 1));

    zvm->function[zvm->function_count] = MEM_malloc(sizeof(Function));
    zvm->function[zvm->function_count]->package_name
        = MEM_strdup(package_name);
    zvm->function[zvm->function_count]->name = MEM_strdup(func_name);
    zvm->function[zvm->function_count]->kind = NATIVE_FUNCTION;
    zvm->function[zvm->function_count]->is_implemented = ZVM_TRUE;
    zvm->function[zvm->function_count]->u.native_f.proc = proc;
    zvm->function[zvm->function_count]->u.native_f.arg_count = arg_count;
    zvm->function[zvm->function_count]->u.native_f.is_method = is_method;
    zvm->function[zvm->function_count]->u.native_f.return_pointer
        = return_pointer;
    zvm->function_count++;
}

void
zvm_dynamic_load(ZVM_VirtualMachine *zvm,
                 ZVM_Executable *caller_exe, Function *caller, int pc,
                 Function *func)
{
    ZEC_Compiler *compiler;
    ZVM_ExecutableItem *pos;
    ZVM_ExecutableItem *add_top;
    SearchFileStatus status;
    char search_file[FILENAME_MAX];

    compiler = ZEC_create_compiler();
    status = zec_dynamic_compile(compiler, func->package_name,
                                 zvm->executable_list, &add_top,
                                 search_file);
    if (status != SEARCH_FILE_SUCCESS) {
        if (status == SEARCH_FILE_NOT_FOUND) {
            zvm_error_i(caller_exe, caller, pc,
                        LOAD_FILE_NOT_FOUND_ERR,
                        ZVM_STRING_MESSAGE_ARGUMENT, "file", search_file,
                        ZVM_MESSAGE_ARGUMENT_END);
        } else {
            zvm_error_i(caller_exe, caller, pc,
                        LOAD_FILE_ERR,
                        ZVM_INT_MESSAGE_ARGUMENT, "status", (int)status,
                        ZVM_MESSAGE_ARGUMENT_END);
        }
    }

    for (pos = add_top; pos; pos = pos->next) {
        add_executable_to_zvm(zvm, pos->executable, ZVM_FALSE);
    }
    ZEC_dispose_compiler(compiler);
}
