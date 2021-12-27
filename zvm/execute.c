#include <math.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

static ZVM_ObjectRef
chain_string(ZVM_VirtualMachine *zvm, ZVM_ObjectRef str1, ZVM_ObjectRef str2)
{
    int result_len;
    ZVM_Char    *left;
    ZVM_Char    *right;
    ZVM_Char    *result;
    ZVM_ObjectRef ret;

    if (str1.data == NULL) {
        left = NULL_STRING;
    } else {
        left = str1.data->u.string.string;
    }
    if (str2.data == NULL) {
        right = NULL_STRING;
    } else {
        right = str2.data->u.string.string;
    }
    result_len = zvm_wcslen(left) + zvm_wcslen(right);
    result = MEM_malloc(sizeof(ZVM_Char) * (result_len + 1));

    zvm_wcscpy(result, left);
    zvm_wcscat(result, right);

    ret = zvm_create_zvm_string_i(zvm, result);

    return ret;
}

static ZVM_Context *
alloc_context(void)
{
    ZVM_Context *ret;

    ret = MEM_malloc(sizeof(ZVM_Context));
    ret->ref_in_native_method = NULL;

    return ret;
}

static void
dispose_context(ZVM_Context *context)
{
    RefInNativeFunc *pos;
    RefInNativeFunc *temp;

    for (pos = context->ref_in_native_method; pos; ) {
        temp = pos->next;
        MEM_free(pos);
        pos = temp;
    }
    MEM_free(context);
}

static void
invoke_native_function(ZVM_VirtualMachine *zvm, Function *callee,
                       int *sp_p)
{
    ZVM_Value   *stack;
    int         sp;
    ZVM_Value   ret;
    int         arg_count;
    ZVM_Context *context;

    stack = zvm->stack.stack;
    sp = *sp_p;
    DBG_assert(callee->kind == NATIVE_FUNCTION,
               ("callee->kind..%d", callee->kind));

    if (callee->u.native_f.is_method) {
        arg_count = callee->u.native_f.arg_count + 1;
    } else {
        arg_count = callee->u.native_f.arg_count;
    }
    context = alloc_context();
    zvm->current_context = context;
    ret = callee->u.native_f.proc(zvm, context,
                                  callee->u.native_f.arg_count,
                                  &stack[sp - arg_count - 1]);
    stack[sp - arg_count-1] = ret;
    zvm->stack.pointer_flags[sp - arg_count-1]
        = callee->u.native_f.return_pointer;
    dispose_context(context);
    zvm->current_context = NULL;

    *sp_p = sp - arg_count;
}

static void
initialize_local_variables(ZVM_VirtualMachine *zvm,
                           ZVM_Function *func, int from_sp)
{
    int i;
    int sp_idx;

    for (i = 0, sp_idx = from_sp; i < func->local_variable_count;
         i++, sp_idx++) {
        zvm->stack.pointer_flags[sp_idx] = ZVM_FALSE;
    }

    for (i = 0, sp_idx = from_sp; i < func->local_variable_count;
         i++, sp_idx++) {
        zvm_initialize_value(func->local_variable[i].type,
                             &zvm->stack.stack[sp_idx]);
        if (is_pointer_type(func->local_variable[i].type)) {
            zvm->stack.pointer_flags[sp_idx] = ZVM_TRUE;
        }
    }
}

static void
expand_stack(ZVM_VirtualMachine *zvm, int need_stack_size)
{
    int revalue_up;
    int rest;

    rest = zvm->stack.alloc_size - zvm->stack.stack_pointer;
    if (rest <= need_stack_size) {
        revalue_up = ((rest / STACK_ALLOC_SIZE) + 1) * STACK_ALLOC_SIZE;

        zvm->stack.alloc_size += revalue_up;
        zvm->stack.stack
            = MEM_realloc(zvm->stack.stack,
                          zvm->stack.alloc_size * sizeof(ZVM_Value));
        zvm->stack.pointer_flags
            = MEM_realloc(zvm->stack.pointer_flags,
                          zvm->stack.alloc_size * sizeof(ZVM_Boolean));
    }
}

static void
invoke_zero_function(ZVM_VirtualMachine *zvm,
                       Function **caller_p, Function *callee,
                       ZVM_Byte **code_p, int *code_size_p, int *pc_p,
                       int *sp_p, int *base_p,
                       ExecutableEntry **ee_p, ZVM_Executable **exe_p)
{
    CallInfo *callInfo;
    ZVM_Function *callee_p;
    int i;

    if (!callee->is_implemented) {
        zvm_dynamic_load(zvm, *exe_p, *caller_p, *pc_p, callee);
    }
    *ee_p = callee->u.zero_f.executable;
    *exe_p = (*ee_p)->executable;
    callee_p = &(*exe_p)->function[callee->u.zero_f.index];

    expand_stack(zvm,
                 CALL_INFO_ALIGN_SIZE
                 + callee_p->local_variable_count
                 + (*exe_p)->function[callee->u.zero_f.index]
                 .code_block.need_stack_size);

    callInfo = (CallInfo*)&zvm->stack.stack[*sp_p-1];
    callInfo->caller = *caller_p;
    callInfo->caller_address = *pc_p;
    callInfo->base = *base_p;
    for (i = 0; i < CALL_INFO_ALIGN_SIZE; i++) {
        zvm->stack.pointer_flags[*sp_p-1+i] = ZVM_FALSE;
    }

    *base_p = *sp_p - callee_p->parameter_count - 1;
    if (callee_p->is_method) {
        (*base_p)--; /* for this */
    }
    *caller_p = callee;

    initialize_local_variables(zvm, callee_p,
                               *sp_p + CALL_INFO_ALIGN_SIZE - 1);

    *sp_p += CALL_INFO_ALIGN_SIZE + callee_p->local_variable_count - 1;
    *pc_p = 0;

    *code_p = (*exe_p)->function[callee->u.zero_f.index].code_block.code;
    *code_size_p
        = (*exe_p)->function[callee->u.zero_f.index].code_block.code_size;
}

/* This function returns ZVM_TRUE if this function was called from native.
 */
static ZVM_Boolean
do_return(ZVM_VirtualMachine *zvm, Function **func_p,
          ZVM_Byte **code_p, int *code_size_p, int *pc_p, int *base_p,
          ExecutableEntry **ee_p, ZVM_Executable **exe_p)
{
    CallInfo *callInfo;
    ZVM_Function *caller_p;
    ZVM_Function *callee_p;
    int arg_count;

    callee_p = &(*exe_p)->function[(*func_p)->u.zero_f.index];

    arg_count = callee_p->parameter_count;
    if (callee_p->is_method) {
        arg_count++; /* for this */
    }
    callInfo = (CallInfo*)&zvm->stack.stack[*base_p + arg_count];

    if (callInfo->caller) {
        *ee_p = callInfo->caller->u.zero_f.executable;
        *exe_p = (*ee_p)->executable;
        caller_p = &(*exe_p)->function[callInfo->caller->u.zero_f.index];
        *code_p = caller_p->code_block.code;
        *code_size_p = caller_p->code_block.code_size;
    } else {
        *ee_p = zvm->top_level;
        *exe_p = zvm->top_level->executable;
        *code_p = zvm->top_level->executable->top_level.code;
        *code_size_p = zvm->top_level->executable->top_level.code_size;
    }
    *func_p = callInfo->caller;

    zvm->stack.stack_pointer = *base_p;
    *pc_p = callInfo->caller_address + 1;
    *base_p = callInfo->base;

    return callInfo->caller_address == CALL_FROM_NATIVE;
}

/* This function returns ZVM_TRUE if this function was called from native.
 */
static ZVM_Boolean
return_function(ZVM_VirtualMachine *zvm, Function **func_p,
                ZVM_Byte **code_p, int *code_size_p, int *pc_p, int *base_p,
                ExecutableEntry **ee_p, ZVM_Executable **exe_p)
{
    ZVM_Value return_value;
    ZVM_Boolean ret;
    ZVM_Function *callee_func;

    return_value = zvm->stack.stack[zvm->stack.stack_pointer-1];
    zvm->stack.stack_pointer--;
    callee_func = &(*exe_p)->function[(*func_p)->u.zero_f.index];

    ret = do_return(zvm, func_p, code_p, code_size_p, pc_p, base_p,
                    ee_p, exe_p);

    zvm->stack.stack[zvm->stack.stack_pointer] = return_value;
    zvm->stack.pointer_flags[zvm->stack.stack_pointer]
        = is_pointer_type(callee_func->type);
    zvm->stack.stack_pointer++;

    return ret;
}

#define STI(zvm, sp) \
  ((zvm)->stack.stack[(zvm)->stack.stack_pointer+(sp)].int_value)
#define STD(zvm, sp) \
  ((zvm)->stack.stack[(zvm)->stack.stack_pointer+(sp)].double_value)
#define STO(zvm, sp) \
  ((zvm)->stack.stack[(zvm)->stack.stack_pointer+(sp)].object)

#define STI_I(zvm, sp) \
  ((zvm)->stack.stack[(sp)].int_value)
#define STD_I(zvm, sp) \
  ((zvm)->stack.stack[(sp)].double_value)
#define STO_I(zvm, sp) \
  ((zvm)->stack.stack[(sp)].object)

#define STI_WRITE(zvm, sp, r) \
  ((zvm)->stack.stack[(zvm)->stack.stack_pointer+(sp)].int_value = r,\
   (zvm)->stack.pointer_flags[(zvm)->stack.stack_pointer+(sp)] = ZVM_FALSE)
#define STD_WRITE(zvm, sp, r) \
  ((zvm)->stack.stack[(zvm)->stack.stack_pointer+(sp)].double_value = r, \
   (zvm)->stack.pointer_flags[(zvm)->stack.stack_pointer+(sp)] = ZVM_FALSE)
#define STO_WRITE(zvm, sp, r) \
  ((zvm)->stack.stack[(zvm)->stack.stack_pointer+(sp)].object = r, \
   (zvm)->stack.pointer_flags[(zvm)->stack.stack_pointer+(sp)] = ZVM_TRUE)

#define STI_WRITE_I(zvm, sp, r) \
  ((zvm)->stack.stack[(sp)].int_value = r,\
   (zvm)->stack.pointer_flags[(sp)] = ZVM_FALSE)
#define STD_WRITE_I(zvm, sp, r) \
  ((zvm)->stack.stack[(sp)].double_value = r, \
   (zvm)->stack.pointer_flags[(sp)] = ZVM_FALSE)
#define STO_WRITE_I(zvm, sp, r) \
  ((zvm)->stack.stack[(sp)].object = r, \
   (zvm)->stack.pointer_flags[(sp)] = ZVM_TRUE)

ZVM_ObjectRef
create_array_sub(ZVM_VirtualMachine *zvm, int dim, int dim_index,
                 ZVM_TypeSpecifier *type)
{
    ZVM_ObjectRef ret;
    int size;
    int i;
    ZVM_ObjectRef exception_dummy;

    size = STI(zvm, -dim);

    if (dim_index == type->derive_count-1) {
        switch (type->basic_type) {
        case ZVM_VOID_TYPE:
            DBG_panic(("creating void array"));
            break;
        case ZVM_BOOLEAN_TYPE: /* FALLTHRU */
        case ZVM_INT_TYPE:
        case ZVM_ENUM_TYPE:
            ret = zvm_create_array_int_i(zvm, size);
            break;
        case ZVM_DOUBLE_TYPE:
            ret = zvm_create_array_double_i(zvm, size);
            break;
        case ZVM_STRING_TYPE: /* FALLTHRU */
        case ZVM_NATIVE_POINTER_TYPE:
        case ZVM_CLASS_TYPE:
        case ZVM_DELEGATE_TYPE:
            ret = zvm_create_array_object_i(zvm, size);
            break;
        case ZVM_NULL_TYPE: /* FALLTHRU */
        case ZVM_BASE_TYPE: /* FALLTHRU */
        case ZVM_UNSPECIFIED_IDENTIFIER_TYPE: /* FALLTHRU */
        default:
            DBG_assert(0, ("type->basic_type..%d\n", type->basic_type));
            break;
        }
    } else if (type->derive[dim_index].tag == ZVM_FUNCTION_DERIVE) {
        DBG_panic(("Function type in array literal.\n"));
    } else {
        ret = zvm_create_array_object_i(zvm, size);
        if (dim_index < dim - 1) {
            STO_WRITE(zvm, 0, ret);
            zvm->stack.stack_pointer++;
            for (i = 0; i < size; i++) {
                ZVM_ObjectRef child;
                child = create_array_sub(zvm, dim, dim_index+1, type);
                ZVM_array_set_object(zvm, ret, i, child, &exception_dummy);
            }
            zvm->stack.stack_pointer--;
        }
    }
    return ret;
}

ZVM_ObjectRef
create_array(ZVM_VirtualMachine *zvm, int dim, ZVM_TypeSpecifier *type)
{
    return create_array_sub(zvm, dim, 0, type);
}

ZVM_ObjectRef
create_array_literal_int(ZVM_VirtualMachine *zvm, int size)
{
    ZVM_ObjectRef array;
    int i;

    array = zvm_create_array_int_i(zvm, size);
    for (i = 0; i < size; i++) {
        array.data->u.array.u.int_array[i] = STI(zvm, -size+i);
    }

    return array;
}

ZVM_ObjectRef
create_array_literal_double(ZVM_VirtualMachine *zvm, int size)
{
    ZVM_ObjectRef array;
    int i;

    array = zvm_create_array_double_i(zvm, size);
    for (i = 0; i < size; i++) {
        array.data->u.array.u.double_array[i] = STD(zvm, -size+i);
    }

    return array;
}

ZVM_ObjectRef
create_array_literal_object(ZVM_VirtualMachine *zvm, int size)
{
    ZVM_ObjectRef array;
    int i;

    array = zvm_create_array_object_i(zvm, size);
    for (i = 0; i < size; i++) {
        array.data->u.array.u.object[i] = STO(zvm, -size+i);
    }

    return array;
}

static void
restore_pc(ZVM_VirtualMachine *zvm, ExecutableEntry *ee,
           Function *func, int pc)
{
    zvm->current_executable = ee;
    zvm->current_function = func;
    zvm->pc = pc;
}

#define is_null_pointer(obj) (((obj)->data == NULL))

static ZVM_Boolean
check_instanceof_i(ZVM_VirtualMachine *zvm, ZVM_ObjectRef *obj,
                   int target_idx,
                   ZVM_Boolean *is_interface, int *interface_idx)
{
    ExecClass *pos;
    int i;

    for (pos = obj->v_table->exec_class->super_class; pos;
         pos = pos->super_class) {
        if (pos->class_index == target_idx) {
            *is_interface = ZVM_FALSE;
            return ZVM_TRUE;
        }
    }

    for (i = 0; i < obj->v_table->exec_class->interface_count; i++) {
        if (obj->v_table->exec_class->interface[i]->class_index
            == target_idx) {
            *is_interface = ZVM_TRUE;
            *interface_idx = i;
            return ZVM_TRUE;
        }
    }
    return ZVM_FALSE;
}

static ZVM_Boolean
check_instanceof(ZVM_VirtualMachine *zvm, ZVM_ObjectRef *obj,
                 int target_idx)
{
    ZVM_Boolean is_interface_dummy;
    int interface_idx_dummy;

    return check_instanceof_i(zvm, obj, target_idx,
                              &is_interface_dummy, &interface_idx_dummy);
}

static ZVM_ErrorStatus
check_down_cast(ZVM_VirtualMachine *zvm, ZVM_ObjectRef *obj, int target_idx,
                ZVM_Boolean *is_same_class,
                ZVM_Boolean *is_interface, int *interface_index)
{
    if (obj->v_table->exec_class->class_index == target_idx) {
        *is_same_class = ZVM_TRUE;
        return ZVM_SUCCESS;
    }
    *is_same_class = ZVM_FALSE;

    if (!check_instanceof_i(zvm, obj, target_idx,
                            is_interface, interface_index)) {
        return ZVM_ERROR;
    }

    return ZVM_SUCCESS;
}

static void
reset_stack_pointer(ZVM_Function *zvm_func, int *sp_p, int base)
{
    if (zvm_func) {
        *sp_p = base + zvm_func->parameter_count
            + (zvm_func->is_method ? 1 : 0)
            + CALL_INFO_ALIGN_SIZE
            + zvm_func->local_variable_count;
    } else {
        *sp_p = 0;
    }
}

/* This function returns ZVM_TRUE, if exception happen in try or catch clause.
 */
static ZVM_Boolean
throw_in_try(ZVM_VirtualMachine *zvm,
             ZVM_Executable *exe, ExecutableEntry *ee, Function *func,
             int *pc_p, int *sp_p, int base)
{
    ZVM_CodeBlock *cb;
    int try_idx;
    int catch_idx;
    ZVM_Boolean throw_in_try = ZVM_FALSE;
    ZVM_Boolean throw_in_catch = ZVM_FALSE;
    int exception_idx;
    ZVM_Function *zvm_func = NULL;

    if (func) {
        cb = &(func->u.zero_f.executable->executable->function
               [func->u.zero_f.index]).code_block;
        zvm_func = &(func->u.zero_f.executable->executable->function
                     [func->u.zero_f.index]);
    } else {
        cb = &exe->top_level;
    }

    exception_idx = zvm->current_exception.v_table->exec_class->class_index;

    for (try_idx = 0; try_idx < cb->try_size; try_idx++) {
        if ((*pc_p) >= cb->try[try_idx].try_start_pc
            && (*pc_p) <= cb->try[try_idx].try_end_pc) {
            throw_in_try = ZVM_TRUE;
            break;
        }
        for (catch_idx = 0; catch_idx < cb->try[try_idx].catch_count;
             catch_idx++) {
            if ((*pc_p) >= cb->try[try_idx].catch_clause[catch_idx].start_pc
                && ((*pc_p)
                    <= cb->try[try_idx].catch_clause[catch_idx].end_pc)) {
                throw_in_catch = ZVM_TRUE;
                break;
            }
        }
    }

    if (try_idx == cb->try_size)
        return ZVM_FALSE;

    DBG_assert(throw_in_try || throw_in_catch, ("bad flags"));

    if (throw_in_try) {
        for (catch_idx = 0; catch_idx < cb->try[try_idx].catch_count;
             catch_idx++) {
            int class_idx_in_exe
                = cb->try[try_idx].catch_clause[catch_idx].class_index;
            int class_idx_in_zvm = ee->class_table[class_idx_in_exe];
            if (exception_idx == class_idx_in_zvm
                || check_instanceof(zvm, &zvm->current_exception,
                                    class_idx_in_zvm)) {
                *pc_p = cb->try[try_idx].catch_clause[catch_idx].start_pc;
                reset_stack_pointer(zvm_func, sp_p, base);
                STO_WRITE(zvm, 0, zvm->current_exception);
                zvm->stack.stack_pointer++;
                zvm->current_exception = zvm_null_object_ref;
                return ZVM_TRUE;
            }
        }
    }
    *pc_p = cb->try[try_idx].finally_start_pc;
    reset_stack_pointer(zvm_func, sp_p, base);
    
    return ZVM_TRUE;
}

static void
add_stack_trace(ZVM_VirtualMachine *zvm, ZVM_Executable *exe,
                Function *func, int pc)
{
    int line_number;
    int class_index;
    ZVM_ObjectRef stack_trace;
    int stack_trace_index;
    ZVM_Object *stack_trace_array;
    int array_size;
    int line_number_index;
    int file_name_index;
    ZVM_Value value;
    ZVM_Char *wc_str;
    char *func_name;
    int func_name_index;

    line_number = zvm_conv_pc_to_line_number(exe, func, pc);
    class_index = zvm_search_class(zvm,
                                   ZERO_DEFAULT_PACKAGE,
                                   ZERO_STACK_TRACE_CLASS);
    stack_trace = zvm_create_class_object_i(zvm, class_index);
    STO_WRITE(zvm, 0, stack_trace);
    zvm->stack.stack_pointer++;
    
    line_number_index
        = ZVM_get_field_index(zvm, stack_trace, "line_number");
    stack_trace.data->u.class_object.field[line_number_index].int_value
        = line_number;

    file_name_index
        = ZVM_get_field_index(zvm, stack_trace, "file_name");
    wc_str = zvm_mbstowcs_alloc(zvm, exe->path);
    stack_trace.data->u.class_object.field[file_name_index].object
        = zvm_create_zvm_string_i(zvm, wc_str);

    func_name_index
        = ZVM_get_field_index(zvm, stack_trace, "function_name");
    if (func) {
        func_name = exe->function[func->u.zero_f.index].name;
    } else {
        func_name = "top level";
    }
    wc_str = zvm_mbstowcs_alloc(zvm, func_name);
    stack_trace.data->u.class_object.field[func_name_index].object
        = zvm_create_zvm_string_i(zvm, wc_str);

    stack_trace_index
        = ZVM_get_field_index(zvm, zvm->current_exception,
                              "stack_trace");
    stack_trace_array = zvm->current_exception.data
        ->u.class_object.field[stack_trace_index].object.data;
    array_size = ZVM_array_size(zvm, stack_trace_array);
    value.object = stack_trace;
    ZVM_array_insert(zvm, stack_trace_array, array_size, value);

    zvm->stack.stack_pointer--;
}

static ZVM_Value execute(ZVM_VirtualMachine *zvm, Function *func,
                         ZVM_Byte *code, int code_size, int base);

static void
do_throw(ZVM_VirtualMachine *zvm,
         Function **func_p, ZVM_Byte **code_p, int *code_size_p, int *pc_p,
         int *base_p, ExecutableEntry **ee_p, ZVM_Executable **exe_p,
         ZVM_ObjectRef *exception)
{
    ZVM_Boolean in_try;
    int i;
    CallInfo *callInfo;

    zvm->current_exception = *exception;
    /* add_stack_trace(zvm, *exe_p, *func_p, *pc_p); */

    for (;;) {
        in_try = throw_in_try(zvm, *exe_p, *ee_p, *func_p, pc_p,
                              &zvm->stack.stack_pointer, *base_p);
        if (in_try)
            break;

        if (*func_p) {
            add_stack_trace(zvm, *exe_p, *func_p, *pc_p);
            do_return(zvm, func_p, code_p, code_size_p, pc_p,
                      base_p, ee_p, exe_p);
        } else {
            int func_index
                = zvm_search_function(zvm,
                                      ZERO_DEFAULT_PACKAGE,
                                      ZERO_PRINT_STACK_TRACE_FUNC);
            ZVM_Executable *zvm_exe;
            ZVM_Function *zvm_func;
            ZVM_Byte *code;
            int code_size;
            int base;

            add_stack_trace(zvm, *exe_p, *func_p, *pc_p);
            zvm_exe = zvm->function[func_index]->u.zero_f.executable
                ->executable;
            code = zvm_exe->function[zvm->function[func_index]
                                     ->u.zero_f.index].code_block.code;
            code_size
                = zvm_exe->function[zvm->function[func_index]
                                    ->u.zero_f.index].code_block.code_size;

            STO_WRITE(zvm, 0, zvm->current_exception);
            zvm->stack.stack_pointer++;
            callInfo = (CallInfo*)&zvm->stack.stack[zvm->stack.stack_pointer];
            callInfo->caller = NULL;
            callInfo->caller_address = CALL_FROM_NATIVE;
            callInfo->base = 0; /*dummy */
            zvm->stack.stack_pointer += CALL_INFO_ALIGN_SIZE;
            for (i = 0; i < CALL_INFO_ALIGN_SIZE; i++) {
                zvm->stack.pointer_flags[zvm->stack.stack_pointer
                                         - CALL_INFO_ALIGN_SIZE + i]
                    = ZVM_FALSE;
            }
            zvm->pc = 0;
            zvm->current_executable
                = zvm->function[func_index]->u.zero_f.executable;

            base = zvm->stack.stack_pointer-1-CALL_INFO_ALIGN_SIZE;

            zvm_func = &zvm_exe->function[zvm->function[func_index]
                                          ->u.zero_f.index];
            initialize_local_variables(zvm, zvm_func,
                                       zvm->stack.stack_pointer);
            zvm->stack.stack_pointer += zvm_func->local_variable_count;

            execute(zvm, zvm->function[func_index],
                    code, code_size, base);

            exit(1);
        }
    }
}

ZVM_ObjectRef
zvm_create_exception(ZVM_VirtualMachine *zvm, char *class_name,
                     RuntimeError id, ...)
{
    int class_index;
    ZVM_ObjectRef obj;
    VString     message;
    va_list     ap;
    int message_index;
    int stack_trace_index;

    va_start(ap, id);
    class_index = zvm_search_class(zvm, ZERO_DEFAULT_PACKAGE,
                                   class_name);
    obj = zvm_create_class_object_i(zvm, class_index);

    STO_WRITE(zvm, 0, obj);
    zvm->stack.stack_pointer++;

    zvm_format_message(zvm_error_message_format, (int)id, &message, ap);
    va_end(ap);

    message_index
        = ZVM_get_field_index(zvm, obj, "message");
    obj.data->u.class_object.field[message_index].object
        = zvm_create_zvm_string_i(zvm, message.string);

    stack_trace_index
        = ZVM_get_field_index(zvm, obj, "stack_trace");
    obj.data->u.class_object.field[stack_trace_index].object
        = zvm_create_array_object_i(zvm, 0);

    zvm->stack.stack_pointer--;

    return obj;
}

static void
throw_null_pointer_exception(ZVM_VirtualMachine *zvm, Function **func_p,
                             ZVM_Byte **code_p, int *code_size_p, int *pc_p,
                             int *base_p,
                             ExecutableEntry **ee_p, ZVM_Executable **exe_p)
{
    ZVM_ObjectRef ex;

    ex = zvm_create_exception(zvm, NULL_POINTER_EXCEPTION_NAME,
                              NULL_POINTER_ERR, ZVM_MESSAGE_ARGUMENT_END);
    STO_WRITE(zvm, 0, ex); /* BUGBUG? irane? */
    zvm->stack.stack_pointer++;
    do_throw(zvm, func_p, code_p, code_size_p, pc_p, base_p,
             ee_p, exe_p, &ex);
}

static void
clear_stack_trace(ZVM_VirtualMachine *zvm, ZVM_ObjectRef *ex)
{
    int stack_trace_index
        = ZVM_get_field_index(zvm, *ex, "stack_trace");

    ex->data->u.class_object.field[stack_trace_index].object
        = zvm_create_array_object_i(zvm, 0);
}

static ZVM_Value
execute(ZVM_VirtualMachine *zvm, Function *func,
        ZVM_Byte *code, int code_size, int base)
{
    ExecutableEntry *ee;
    ZVM_Executable *exe;
    int         pc;
    ZVM_Value   ret;

    pc = zvm->pc;
    ee = zvm->current_executable;
    exe = zvm->current_executable->executable;

    while (pc < code_size) {
        /*
        zvm_dump_instruction(stderr, code, pc);
        fprintf(stderr, "\tsp(%d)\n", zvm->stack.stack_pointer);
        */
        switch ((ZVM_Opcode)code[pc]) {
        case ZVM_PUSH_INT_1BYTE:
            STI_WRITE(zvm, 0, code[pc+1]);
            zvm->stack.stack_pointer++;
            pc += 2;
            break;
        case ZVM_PUSH_INT_2BYTE:
            STI_WRITE(zvm, 0, GET_2BYTE_INT(&code[pc+1]));
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_INT:
            STI_WRITE(zvm, 0,
                      exe->constant_pool[GET_2BYTE_INT(&code[pc+1])]
                      .u.c_int);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_DOUBLE_0:
            STD_WRITE(zvm, 0, 0.0);
            zvm->stack.stack_pointer++;
            pc++;
            break;
        case ZVM_PUSH_DOUBLE_1:
            STD_WRITE(zvm, 0, 1.0);
            zvm->stack.stack_pointer++;
            pc++;
            break;
        case ZVM_PUSH_DOUBLE:
            STD_WRITE(zvm, 0, 
                      exe->constant_pool[GET_2BYTE_INT(&code[pc+1])]
                      .u.c_double);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_STRING:
            STO_WRITE(zvm, 0,
                      zvm_literal_to_zvm_string_i(zvm,
                                                  exe->constant_pool
                                                  [GET_2BYTE_INT(&code
                                                                 [pc+1])]
                                                  .u.c_string));
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_NULL:
            STO_WRITE(zvm, 0, zvm_null_object_ref);
            zvm->stack.stack_pointer++;
            pc++;
            break;
        case ZVM_PUSH_STACK_INT:
            STI_WRITE(zvm, 0,
                      STI_I(zvm, base + GET_2BYTE_INT(&code[pc+1])));
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_STACK_DOUBLE:
            STD_WRITE(zvm, 0,
                      STD_I(zvm, base + GET_2BYTE_INT(&code[pc+1])));
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_STACK_OBJECT:
            STO_WRITE(zvm, 0,
                      STO_I(zvm, base + GET_2BYTE_INT(&code[pc+1])));
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_POP_STACK_INT:
            STI_WRITE_I(zvm, base + GET_2BYTE_INT(&code[pc+1]),
                        STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc += 3;
            break;
        case ZVM_POP_STACK_DOUBLE:
            STD_WRITE_I(zvm, base + GET_2BYTE_INT(&code[pc+1]),
                        STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc += 3;
            break;
        case ZVM_POP_STACK_OBJECT:
            STO_WRITE_I(zvm, base + GET_2BYTE_INT(&code[pc+1]),
                        STO(zvm, -1));
            zvm->stack.stack_pointer--;
            pc += 3;
            break;
        case ZVM_PUSH_STATIC_INT:
            STI_WRITE(zvm, 0,
                      ee->static_v.variable[GET_2BYTE_INT(&code[pc+1])]
                      .int_value);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_STATIC_DOUBLE:
            STD_WRITE(zvm, 0,
                      ee->static_v.variable[GET_2BYTE_INT(&code[pc+1])]
                      .double_value);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_PUSH_STATIC_OBJECT:
            STO_WRITE(zvm, 0,
                      ee->static_v.variable[GET_2BYTE_INT(&code[pc+1])]
                      .object);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        case ZVM_POP_STATIC_INT:
            ee->static_v.variable[GET_2BYTE_INT(&code[pc+1])].int_value
                = STI(zvm, -1);
            zvm->stack.stack_pointer--;
            pc += 3;
            break;
        case ZVM_POP_STATIC_DOUBLE:
            ee->static_v.variable[GET_2BYTE_INT(&code[pc+1])]
                .double_value
                = STD(zvm, -1);
            zvm->stack.stack_pointer--;
            pc += 3;
            break;
        case ZVM_POP_STATIC_OBJECT:
            ee->static_v.variable[GET_2BYTE_INT(&code[pc+1])].object
                = STO(zvm, -1);
            zvm->stack.stack_pointer--;
            pc += 3;
            break;
        case ZVM_PUSH_ARRAY_INT:
        {
            ZVM_ObjectRef array = STO(zvm, -2);
            int index = STI(zvm, -1);
            int int_value;
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            restore_pc(zvm, ee, func, pc);

            status = ZVM_array_get_int(zvm, array, index,
                                       &int_value, &exception);
            if (status == ZVM_SUCCESS) {
                STI_WRITE(zvm, -2, int_value);
                zvm->stack.stack_pointer--;
                pc++;
            } else {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            }
            break;
        }
        case ZVM_PUSH_ARRAY_DOUBLE:
        {
            ZVM_ObjectRef array = STO(zvm, -2);
            int index = STI(zvm, -1);
            double double_value;
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            restore_pc(zvm, ee, func, pc);
            status = ZVM_array_get_double(zvm, array, index,
                                          &double_value, &exception);
            if (status == ZVM_SUCCESS) {
                STD_WRITE(zvm, -2, double_value);
                zvm->stack.stack_pointer--;
                pc++;
            } else {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            }
            break;
        }
        case ZVM_PUSH_ARRAY_OBJECT:
        {
            ZVM_ObjectRef array = STO(zvm, -2);
            int index = STI(zvm, -1);
            ZVM_ObjectRef object;
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            restore_pc(zvm, ee, func, pc);
            status = ZVM_array_get_object(zvm, array, index,
                                          &object, &exception);
            if (status == ZVM_SUCCESS) {
                STO_WRITE(zvm, -2, object);
                zvm->stack.stack_pointer--;
                pc++;
            } else {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            }
            break;
        }
        case ZVM_POP_ARRAY_INT:
        {
            int value = STI(zvm, -3);
            ZVM_ObjectRef array = STO(zvm, -2);
            int index = STI(zvm, -1);
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            restore_pc(zvm, ee, func, pc);
            status = ZVM_array_set_int(zvm, array, index, value, &exception);
            if (status == ZVM_SUCCESS) {
                zvm->stack.stack_pointer -= 3;
                pc++;
            } else {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            }
            break;
        }
        case ZVM_POP_ARRAY_DOUBLE:
        {
            double value = STD(zvm, -3);
            ZVM_ObjectRef array = STO(zvm, -2);
            int index = STI(zvm, -1);
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            restore_pc(zvm, ee, func, pc);
            status
                = ZVM_array_set_double(zvm, array, index, value, &exception);
            if (status == ZVM_SUCCESS) {
                zvm->stack.stack_pointer -= 3;
                pc++;
            } else {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            }
            break;
        }
        case ZVM_POP_ARRAY_OBJECT:
        {
            ZVM_ObjectRef value = STO(zvm, -3);
            ZVM_ObjectRef array = STO(zvm, -2);
            int index = STI(zvm, -1);
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            restore_pc(zvm, ee, func, pc);
            status
                = ZVM_array_set_object(zvm, array, index, value, &exception);
            if (status == ZVM_SUCCESS) {
                zvm->stack.stack_pointer -= 3;
                pc++;
            } else {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            }
            break;
        }
        case ZVM_PUSH_FIELD_INT:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                STI_WRITE(zvm, -1,
                          obj.data->u.class_object.field[index].int_value);
                pc += 3;
            }
            break;
        }
        case ZVM_PUSH_FIELD_DOUBLE:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                STD_WRITE(zvm, -1,
                          obj.data->u.class_object.field[index].double_value);
                pc += 3;
            }
            break;
        }
        case ZVM_PUSH_FIELD_OBJECT:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                STO_WRITE(zvm, -1,
                          obj.data->u.class_object.field[index].object);
                pc += 3;
            }
            break;
        }
        case ZVM_POP_FIELD_INT:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                obj.data->u.class_object.field[index].int_value
                    = STI(zvm, -2);
                zvm->stack.stack_pointer -= 2;
                pc += 3;
            }
            break;
        }
        case ZVM_POP_FIELD_DOUBLE:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                obj.data->u.class_object.field[index].double_value
                    = STD(zvm, -2);
                zvm->stack.stack_pointer -= 2;
                pc += 3;
            }
            break;
        }
        case ZVM_POP_FIELD_OBJECT:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                obj.data->u.class_object.field[index].object = STO(zvm, -2);
                zvm->stack.stack_pointer -= 2;
                pc += 3;
            }
            break;
        }
        case ZVM_ADD_INT:
            STI(zvm, -2) = STI(zvm, -2) + STI(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_ADD_DOUBLE:
            STD(zvm, -2) = STD(zvm, -2) + STD(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_ADD_STRING:
            STO(zvm, -2) = chain_string(zvm,
                                        STO(zvm, -2),
                                        STO(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_SUB_INT:
            STI(zvm, -2) = STI(zvm, -2) - STI(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_SUB_DOUBLE:
            STD(zvm, -2) = STD(zvm, -2) - STD(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_MUL_INT:
            STI(zvm, -2) = STI(zvm, -2) * STI(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_MUL_DOUBLE:
            STD(zvm, -2) = STD(zvm, -2) * STD(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_DIV_INT:
            if (STI(zvm, -1) == 0) {
                ZVM_ObjectRef exception;

                exception
                    = zvm_create_exception(zvm,
                                           DIVISION_BY_ZERO_EXCEPTION_NAME,
                                           DIVISION_BY_ZERO_ERR,
                                           ZVM_MESSAGE_ARGUMENT_END);
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &exception);
            } else {
                STI(zvm, -2) = STI(zvm, -2) / STI(zvm, -1);
                zvm->stack.stack_pointer--;
                pc++;
            }
            break;
        case ZVM_DIV_DOUBLE:
            STD(zvm, -2) = STD(zvm, -2) / STD(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_MOD_INT:
            STI(zvm, -2) = STI(zvm, -2) % STI(zvm, -1);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_MOD_DOUBLE:
            STD(zvm, -2) = fmod(STD(zvm, -2), STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_MINUS_INT:
            STI(zvm, -1) = -STI(zvm, -1);
            pc++;
            break;
        case ZVM_MINUS_DOUBLE:
            STD(zvm, -1) = -STD(zvm, -1);
            pc++;
            break;
        case ZVM_INCREMENT:
            STI(zvm, -1)++;
            pc++;
            break;
        case ZVM_DECREMENT:
            STI(zvm, -1)--;
            pc++;
            break;
        case ZVM_CAST_INT_TO_DOUBLE:
            STD(zvm, -1) = (double)STI(zvm, -1);
            pc++;
            break;
        case ZVM_CAST_DOUBLE_TO_INT:
            STI(zvm, -1) = (int)STD(zvm, -1);
            pc++;
            break;
        case ZVM_CAST_BOOLEAN_TO_STRING:
            if (STI(zvm, -1)) {
                STO_WRITE(zvm, -1,
                          zvm_literal_to_zvm_string_i(zvm, TRUE_STRING));
            } else {
                STO_WRITE(zvm, -1,
                          zvm_literal_to_zvm_string_i(zvm, FALSE_STRING));
            }
            pc++;
            break;
        case ZVM_CAST_INT_TO_STRING:
        {
            char buf[LINE_BUF_SIZE];
            ZVM_Char *wc_str;

            sprintf(buf, "%d", STI(zvm, -1));
            restore_pc(zvm, ee, func, pc);
            wc_str = zvm_mbstowcs_alloc(zvm, buf);
            STO_WRITE(zvm, -1,
                      zvm_create_zvm_string_i(zvm, wc_str));
            pc++;
            break;
        }
        case ZVM_CAST_DOUBLE_TO_STRING:
        {
            char buf[LINE_BUF_SIZE];
            ZVM_Char *wc_str;

            sprintf(buf, "%f", STD(zvm, -1));
            restore_pc(zvm, ee, func, pc);
            wc_str = zvm_mbstowcs_alloc(zvm, buf);
            STO_WRITE(zvm, -1,
                      zvm_create_zvm_string_i(zvm, wc_str));
            pc++;
            break;
        }
        case ZVM_CAST_ENUM_TO_STRING:
        {
            ZVM_Char *wc_str;
            int enum_index = GET_2BYTE_INT(&code[pc+1]);
            int enum_value = STI(zvm, -1);

            restore_pc(zvm, ee, func, pc);
            wc_str = zvm_mbstowcs_alloc(zvm,
                                        exe->enum_definition[enum_index]
                                        .enumerator[enum_value]);
            STO_WRITE(zvm, -1,
                      zvm_create_zvm_string_i(zvm, wc_str));
            pc += 3;
            break;
        }
        case ZVM_UP_CAST:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                obj.v_table
                    = obj.v_table->exec_class->interface_v_table[index];
                STO_WRITE(zvm, -1, obj);
                pc += 3;
            }
            break;
        }
        case ZVM_DOWN_CAST:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int idx_in_exe = GET_2BYTE_INT(&code[pc+1]);
            int index = ee->class_table[idx_in_exe];
            ZVM_Boolean is_same_class;
            ZVM_Boolean is_interface;
            int interface_idx;
            ZVM_ErrorStatus status;
            ZVM_ObjectRef exception;

            do {
                if (is_null_pointer(&obj)) {
                    throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                                 &pc, &base, &ee, &exe);
                    break;
                }
                status = check_down_cast(zvm, &obj, index,
                                         &is_same_class,
                                         &is_interface, &interface_idx);
                if (status != ZVM_SUCCESS) {
                    exception
                        = zvm_create_exception(zvm,
                                               CLASS_CAST_EXCEPTION_NAME,
                                               CLASS_CAST_ERR,
                                               ZVM_STRING_MESSAGE_ARGUMENT,
                                               "org",
                                               obj.v_table->exec_class->name,
                                               ZVM_STRING_MESSAGE_ARGUMENT,
                                               "target",
                                               zvm->class[index]->name,
                                               ZVM_MESSAGE_ARGUMENT_END);
                    do_throw(zvm, &func, &code, &code_size, &pc,
                             &base, &ee, &exe, &exception);
                    break;
                }
                if (!is_same_class) {
                    if (is_interface) {
                        obj.v_table
                            = obj.v_table->exec_class
                            ->interface_v_table[interface_idx];
                    } else {
                        obj.v_table = obj.v_table->exec_class->class_table;
                    }
                }
                STO_WRITE(zvm, -1, obj);
                pc += 3;
            } while (0);
            break;
        }
        case ZVM_EQ_INT:
            STI(zvm, -2) = (STI(zvm, -2) == STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_EQ_DOUBLE:
            STI(zvm, -2) = (STD(zvm, -2) == STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_EQ_OBJECT:
            STI_WRITE(zvm, -2, STO(zvm, -2).data == STO(zvm, -1).data);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_EQ_STRING:
            STI_WRITE(zvm, -2,
                      !zvm_wcscmp(STO(zvm, -2).data->u.string.string,
                                  STO(zvm, -1).data->u.string.string));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_GT_INT:
            STI(zvm, -2) = (STI(zvm, -2) > STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_GT_DOUBLE:
            STI(zvm, -2) = (STD(zvm, -2) > STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_GT_STRING:
            STI_WRITE(zvm, -2,
                      zvm_wcscmp(STO(zvm, -2).data->u.string.string,
                                 STO(zvm, -1).data->u.string.string) > 0);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_GE_INT:
            STI(zvm, -2) = (STI(zvm, -2) >= STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_GE_DOUBLE:
            STI(zvm, -2) = (STD(zvm, -2) >= STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_GE_STRING:
            STI_WRITE(zvm, -2,
                      zvm_wcscmp(STO(zvm, -2).data->u.string.string,
                                 STO(zvm, -1).data->u.string.string) >= 0);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LT_INT:
            STI(zvm, -2) = (STI(zvm, -2) < STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LT_DOUBLE:
            STI(zvm, -2) = (STD(zvm, -2) < STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LT_STRING:
            STI_WRITE(zvm, -2,
                      zvm_wcscmp(STO(zvm, -2).data->u.string.string,
                                 STO(zvm, -1).data->u.string.string) < 0);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LE_INT:
            STI(zvm, -2) = (STI(zvm, -2) <= STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LE_DOUBLE:
            STI(zvm, -2) = (STD(zvm, -2) <= STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LE_STRING:
            STI_WRITE(zvm, -2,
                      zvm_wcscmp(STO(zvm, -2).data->u.string.string,
                                 STO(zvm, -1).data->u.string.string) <= 0);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_NE_INT:
            STI(zvm, -2) = (STI(zvm, -2) != STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_NE_DOUBLE:
            STI(zvm, -2) = (STD(zvm, -2) != STD(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_NE_OBJECT:
            STI_WRITE(zvm, -2, STO(zvm, -2).data != STO(zvm, -1).data);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_NE_STRING:
            STI_WRITE(zvm, -2,
                      zvm_wcscmp(STO(zvm, -2).data->u.string.string,
                                 STO(zvm, -1).data->u.string.string) != 0);
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LOGICAL_AND:
            STI(zvm, -2) = (STI(zvm, -2) && STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LOGICAL_OR:
            STI(zvm, -2) = (STI(zvm, -2) || STI(zvm, -1));
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_LOGICAL_NOT:
            STI(zvm, -1) = !STI(zvm, -1);
            pc++;
            break;
        case ZVM_POP:
            zvm->stack.stack_pointer--;
            pc++;
            break;
        case ZVM_DUPLICATE:
            zvm->stack.stack[zvm->stack.stack_pointer]
                = zvm->stack.stack[zvm->stack.stack_pointer-1];
            zvm->stack.pointer_flags[zvm->stack.stack_pointer]
                = zvm->stack.pointer_flags[zvm->stack.stack_pointer-1];
            zvm->stack.stack_pointer++;
            pc++;
            break;
        case ZVM_DUPLICATE_OFFSET:
            {
                int offset = GET_2BYTE_INT(&code[pc+1]);
                zvm->stack.stack[zvm->stack.stack_pointer]
                    = zvm->stack.stack[zvm->stack.stack_pointer-1-offset];
                zvm->stack.pointer_flags[zvm->stack.stack_pointer]
                    = zvm->stack.pointer_flags[zvm->stack.stack_pointer-1
                                               -offset];
                zvm->stack.stack_pointer++;
                pc += 3;
                break;
            }
        case ZVM_JUMP:
            pc = GET_2BYTE_INT(&code[pc+1]);
            break;
        case ZVM_JUMP_IF_TRUE:
            if (STI(zvm, -1)) {
                pc = GET_2BYTE_INT(&code[pc+1]);
            } else {
                pc += 3;
            }
            zvm->stack.stack_pointer--;
            break;
        case ZVM_JUMP_IF_FALSE:
            if (!STI(zvm, -1)) {
                pc = GET_2BYTE_INT(&code[pc+1]);
            } else {
                pc += 3;
            }
            zvm->stack.stack_pointer--;
            break;
        case ZVM_PUSH_FUNCTION:
        {
            int idx_in_exe = GET_2BYTE_INT(&code[pc+1]);
            STI_WRITE(zvm, 0, ee->function_table[idx_in_exe]);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        }
        case ZVM_PUSH_METHOD:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);

            if (is_null_pointer(&obj)) {
                throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                             &pc, &base, &ee, &exe);
            } else {
                STI_WRITE(zvm, 0, obj.v_table->table[index].index);
                zvm->stack.stack_pointer++;
                pc += 3;
            }
            break;
        }
        case ZVM_PUSH_DELEGATE:
        {
            ZVM_ObjectRef obj = STO(zvm, -1);
            int index = GET_2BYTE_INT(&code[pc+1]);
            ZVM_ObjectRef delegate;
            
            delegate = zvm_create_delegate(zvm, obj, index);
            STO_WRITE(zvm, -1, delegate);
            pc += 3;

            break;
        }
        case ZVM_INVOKE: /* FALLTHRU */
        case ZVM_INVOKE_DELEGATE:
        {
            int func_idx;

            if ((ZVM_Opcode)code[pc] == ZVM_INVOKE_DELEGATE) {
                ZVM_ObjectRef delegate = STO(zvm, -1);

                if (is_null_pointer(&delegate)) {
                    throw_null_pointer_exception(zvm, &func, &code, &code_size,
                                                 &pc, &base, &ee, &exe);
                }
                if (is_null_pointer(&delegate.data->u.delegate.object)) {
                    func_idx
                        = ee->function_table[delegate.data->u.delegate.index];
                } else {
                    func_idx
                        = (delegate.data->u.delegate.object.v_table
                           ->table[delegate.data->u.delegate.index].index);
                    STO_WRITE(zvm, -1, delegate.data->u.delegate.object);
                    zvm->stack.stack_pointer++; /* for func index */
                }
            } else {
                func_idx = STI(zvm, -1);
            }
            if (zvm->function[func_idx]->kind == NATIVE_FUNCTION) {
                restore_pc(zvm, ee, func, pc);
                zvm->current_exception = zvm_null_object_ref;
                invoke_native_function(zvm, zvm->function[func_idx],
                                       &zvm->stack.stack_pointer);
                if (!is_object_null(zvm->current_exception)) {
                    do_throw(zvm, &func, &code, &code_size, &pc,
                             &base, &ee, &exe, &zvm->current_exception);
                } else {
                    pc++;
                }
            } else {
                invoke_zero_function(zvm, &func, zvm->function[func_idx],
                                       &code, &code_size, &pc,
                                       &zvm->stack.stack_pointer, &base,
                                       &ee, &exe);
            }
            break;
        }
        case ZVM_RETURN:
            if (return_function(zvm, &func, &code, &code_size, &pc,
                                &base, &ee, &exe)) {
                goto EXECUTE_END;
            }
            break;
        case ZVM_NEW:
        {
            int idx_in_exe = GET_2BYTE_INT(&code[pc+1]);
            int class_index = ee->class_table[idx_in_exe];
            STO_WRITE(zvm, 0, zvm_create_class_object_i(zvm, class_index));
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        }
        case ZVM_NEW_ARRAY:
        {
            int dim = code[pc+1];
            ZVM_TypeSpecifier *type
                = &exe->type_specifier[GET_2BYTE_INT(&code[pc+2])];
            ZVM_ObjectRef array;

            restore_pc(zvm, ee, func, pc);
            array = create_array(zvm, dim, type);
            zvm->stack.stack_pointer -= dim;
            STO_WRITE(zvm, 0, array);
            zvm->stack.stack_pointer++;
            pc += 4;
            break;
        }
        case ZVM_NEW_ARRAY_LITERAL_INT:
        {
            int size = GET_2BYTE_INT(&code[pc+1]);
            ZVM_ObjectRef array;

            restore_pc(zvm, ee, func, pc);
            array = create_array_literal_int(zvm, size);
            zvm->stack.stack_pointer -= size;
            STO_WRITE(zvm, 0, array);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        }
        case ZVM_NEW_ARRAY_LITERAL_DOUBLE:
        {
            int size = GET_2BYTE_INT(&code[pc+1]);
            ZVM_ObjectRef array;

            restore_pc(zvm, ee, func, pc);
            array = create_array_literal_double(zvm, size);
            zvm->stack.stack_pointer -= size;
            STO_WRITE(zvm, 0, array);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        }
        case ZVM_NEW_ARRAY_LITERAL_OBJECT:
        {
            int size = GET_2BYTE_INT(&code[pc+1]);
            ZVM_ObjectRef array;

            restore_pc(zvm, ee, func, pc);
            array = create_array_literal_object(zvm, size);
            zvm->stack.stack_pointer -= size;
            STO_WRITE(zvm, 0, array);
            zvm->stack.stack_pointer++;
            pc += 3;
            break;
        }
        case ZVM_SUPER:
        {
            ZVM_ObjectRef* obj = &STO(zvm, -1);
            ExecClass *this_class;
            
            this_class = obj->v_table->exec_class;
            obj->v_table = this_class->super_class->class_table;
            pc++;
            break;
        }
        case ZVM_INSTANCEOF:
        {
            ZVM_ObjectRef* obj = &STO(zvm, -1);
            int idx_in_exe = GET_2BYTE_INT(&code[pc+1]);
            int target_idx = ee->class_table[idx_in_exe];

            if (obj->v_table->exec_class->class_index == target_idx) {
                STI_WRITE(zvm, -1, ZVM_TRUE);
            } else {
                STI_WRITE(zvm, -1, check_instanceof(zvm, obj, target_idx));
            }
            pc += 3;
            break;
        }
        case ZVM_THROW:
        {
            ZVM_ObjectRef* exception = &STO(zvm, -1);

            clear_stack_trace(zvm, exception);
            do_throw(zvm, &func, &code, &code_size, &pc,
                     &base, &ee, &exe, exception);
            break;
        }
        case ZVM_RETHROW:
        {
            ZVM_ObjectRef* exception = &STO(zvm, -1);

            do_throw(zvm, &func, &code, &code_size, &pc,
                     &base, &ee, &exe, exception);
            break;
        }
        case ZVM_GO_FINALLY:
            STI_WRITE(zvm, 0, pc);
            zvm->stack.stack_pointer++;
            pc = GET_2BYTE_INT(&code[pc+1]);
            break;
        case ZVM_FINALLY_END:
            if (!is_object_null(zvm->current_exception)) {
                do_throw(zvm, &func, &code, &code_size, &pc,
                         &base, &ee, &exe, &zvm->current_exception);
            } else {
                pc = STI(zvm, -1) + 3;
                zvm->stack.stack_pointer--;
            }
            break;
        default:
            DBG_assert(0, ("code[%d]..%d\n", pc, code[pc]));
        }
        /* MEM_check_all_blocks(); */
    }
 EXECUTE_END:
    ;
    return ret;
}


ZVM_Value
ZVM_execute(ZVM_VirtualMachine *zvm)
{
    ZVM_Value ret;

    zvm->current_executable = zvm->top_level;
    zvm->current_function = NULL;
    zvm->pc = 0;
    expand_stack(zvm, zvm->top_level->executable->top_level.need_stack_size);
    execute(zvm, NULL, zvm->top_level->executable->top_level.code,
            zvm->top_level->executable->top_level.code_size, 0);

    return ret;
}

void ZVM_dispose_executable_list(ZVM_ExecutableList *list)
{
    ZVM_ExecutableItem *temp;

    while (list->list) {
        temp = list->list;
        list->list = temp->next;
        zvm_dispose_executable(temp->executable);
        MEM_free(temp);
    }
    MEM_free(list);
}

static void
dispose_v_table(ZVM_VTable *v_table)
{
    int i;

    for (i = 0; i < v_table->table_size; i++) {
        MEM_free(v_table->table[i].name);
    }
    MEM_free(v_table->table);
    MEM_free(v_table);
}

void
ZVM_dispose_virtual_machine(ZVM_VirtualMachine *zvm)
{
    ExecutableEntry *ee_temp;
    int i;
    int j;

    while (zvm->executable_entry) {
        ee_temp = zvm->executable_entry;
        zvm->executable_entry = ee_temp->next;

        MEM_free(ee_temp->function_table);
        MEM_free(ee_temp->class_table);
        MEM_free(ee_temp->static_v.variable);
        MEM_free(ee_temp);
    }
    zvm_garbage_collect(zvm);

    MEM_free(zvm->stack.stack);
    MEM_free(zvm->stack.pointer_flags);


    for (i = 0; i < zvm->function_count; i++) {
        MEM_free(zvm->function[i]->name);
        MEM_free(zvm->function[i]->package_name);
        MEM_free(zvm->function[i]);
    }
    MEM_free(zvm->function);

    for (i = 0; i < zvm->class_count; i++) {
        MEM_free(zvm->class[i]->package_name);
        MEM_free(zvm->class[i]->name);
        dispose_v_table(zvm->class[i]->class_table);
        for (j = 0; j < zvm->class[i]->interface_count; j++) {
            dispose_v_table(zvm->class[i]->interface_v_table[j]);
        }
        MEM_free(zvm->class[i]->interface_v_table);
        MEM_free(zvm->class[i]->interface);
        MEM_free(zvm->class[i]->field_type);
        MEM_free(zvm->class[i]);
    }
    MEM_free(zvm->array_v_table->table);
    MEM_free(zvm->array_v_table);
    MEM_free(zvm->string_v_table->table);
    MEM_free(zvm->string_v_table);
    MEM_free(zvm->class);

    MEM_free(zvm);
}
