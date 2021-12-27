#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

static ZVM_ErrorStatus
check_array(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
            ZVM_Executable *exe, Function *func, int pc,
            ZVM_ObjectRef *exception_p)
{
    if (array.data == NULL) {
        *exception_p
            = zvm_create_exception(zvm, NULL_POINTER_EXCEPTION_NAME,
                                   NULL_POINTER_ERR,
                                   ZVM_MESSAGE_ARGUMENT_END);
        return ZVM_ERROR;
    }
    if (index < 0 || index >= array.data->u.array.size) {
        *exception_p
            = zvm_create_exception(zvm, ARRAY_INDEX_EXCEPTION_NAME,
                                   INDEX_OUT_OF_BOUNDS_ERR,
                                   ZVM_INT_MESSAGE_ARGUMENT, "index", index,
                                   ZVM_INT_MESSAGE_ARGUMENT, "size",
                                   array.data->u.array.size,
                                   ZVM_MESSAGE_ARGUMENT_END);
        return ZVM_ERROR;
    }
    return ZVM_SUCCESS;
}

ZVM_ErrorStatus
ZVM_array_get_int(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                  int *value, ZVM_ObjectRef *exception_p)
{
    ZVM_ErrorStatus status;
    status = check_array(zvm, array, index,
                         zvm->current_executable->executable,
                         zvm->current_function, zvm->pc, exception_p);
    if (status != ZVM_SUCCESS) {
        return status;
    }

    *value = array.data->u.array.u.int_array[index];

    return ZVM_SUCCESS;
}

ZVM_ErrorStatus
ZVM_array_get_double(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array,
                     int index, double *value, ZVM_ObjectRef *exception_p)
{
    ZVM_ErrorStatus status;

    status = check_array(zvm, array, index,
                         zvm->current_executable->executable,
                         zvm->current_function, zvm->pc, exception_p);
    if (status != ZVM_SUCCESS) {
        return status;
    }

    *value = array.data->u.array.u.double_array[index];

    return ZVM_SUCCESS;
}

ZVM_ErrorStatus
ZVM_array_get_object(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                     ZVM_ObjectRef *value, ZVM_ObjectRef *exception_p)
{
    ZVM_ErrorStatus status;

    status = check_array(zvm, array, index,
                         zvm->current_executable->executable,
                         zvm->current_function, zvm->pc, exception_p);
    if (status != ZVM_SUCCESS) {
        return status;
    }

    *value = array.data->u.array.u.object[index];

    return ZVM_SUCCESS;
}

ZVM_ErrorStatus
ZVM_array_set_int(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                  int value, ZVM_ObjectRef *exception_p)
{
    ZVM_ErrorStatus status;

    status = check_array(zvm, array, index,
                         zvm->current_executable->executable,
                         zvm->current_function, zvm->pc, exception_p);
    if (status != ZVM_SUCCESS) {
        return status;
    }

    array.data->u.array.u.int_array[index] = value;

    return ZVM_SUCCESS;
}

ZVM_ErrorStatus
ZVM_array_set_double(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                     double value, ZVM_ObjectRef *exception_p)
{
    ZVM_ErrorStatus status;

    status = check_array(zvm, array, index,
                         zvm->current_executable->executable,
                         zvm->current_function, zvm->pc, exception_p);
    if (status != ZVM_SUCCESS) {
        return status;
    }

    array.data->u.array.u.double_array[index] = value;

    return ZVM_SUCCESS;
}

ZVM_ErrorStatus
ZVM_array_set_object(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                     ZVM_ObjectRef value, ZVM_ObjectRef *exception_p)
{
    ZVM_ErrorStatus status;

    status = check_array(zvm, array, index,
                         zvm->current_executable->executable,
                         zvm->current_function, zvm->pc, exception_p);
    if (status != ZVM_SUCCESS) {
        return status;
    }

    array.data->u.array.u.object[index] = value;

    return ZVM_SUCCESS;
}

int
ZVM_array_size(ZVM_VirtualMachine *zvm, ZVM_Object *array)
{
    return array->u.array.size;
}

/* This function doesn't update array->u.array.size.
 */
static void
resize_array(ZVM_VirtualMachine *zvm, ZVM_Object *array, int new_size)
{
    int new_alloc_size;
    ZVM_Boolean need_realloc;

    DBG_assert(array->type == ARRAY_OBJECT, ("array->type..%d", array->type));

    if (new_size > array->u.array.alloc_size) {
        new_alloc_size = array->u.array.alloc_size * 2;
        if (new_alloc_size < new_size) {
            new_alloc_size = new_size + ARRAY_ALLOC_SIZE;
        } else if (new_alloc_size - array->u.array.alloc_size
                   > ARRAY_ALLOC_SIZE) {
            new_alloc_size = array->u.array.alloc_size + ARRAY_ALLOC_SIZE;
        }
        need_realloc = ZVM_TRUE;
    } else if (array->u.array.alloc_size - new_size > ARRAY_ALLOC_SIZE) {
        new_alloc_size = new_size;
        need_realloc = ZVM_TRUE;
    } else {
        need_realloc = ZVM_FALSE;
    }
    if (need_realloc) {
        ZVM_check_gc(zvm);
        switch (array->u.array.type) {
        case INT_ARRAY:
            array->u.array.u.int_array
                = MEM_realloc(array->u.array.u.int_array,
                              new_alloc_size * sizeof(int));
            zvm->heap.current_heap_size
                += (new_alloc_size - array->u.array.alloc_size)
                * sizeof(int);
            break;
        case DOUBLE_ARRAY:
            array->u.array.u.double_array
                = MEM_realloc(array->u.array.u.double_array,
                              new_alloc_size * sizeof(double));
            zvm->heap.current_heap_size
                += (new_alloc_size - array->u.array.alloc_size)
                * sizeof(double);
            break;
        case OBJECT_ARRAY:
            array->u.array.u.object
                = MEM_realloc(array->u.array.u.object,
                              new_alloc_size * sizeof(ZVM_ObjectRef));
            zvm->heap.current_heap_size
                += (new_alloc_size - array->u.array.alloc_size)
                * sizeof(ZVM_ObjectRef);
            break;
        case FUNCTION_INDEX_ARRAY:
        default:
            DBG_panic(("array->u.array.type..%d", array->u.array.type));
        }
        array->u.array.alloc_size = new_alloc_size;
    }
}

void
ZVM_array_resize(ZVM_VirtualMachine *zvm, ZVM_Object *array, int new_size)
{
    int i;    

    resize_array(zvm, array, new_size);

    switch (array->u.array.type) {
    case INT_ARRAY:
        for (i = array->u.array.size; i < new_size; i++) {
            array->u.array.u.int_array[i] = 0;
        }
        break;
    case DOUBLE_ARRAY:
        for (i = array->u.array.size; i < new_size; i++) {
            array->u.array.u.double_array[i] = 0;
        }
        break;
    case OBJECT_ARRAY:
        for (i = array->u.array.size; i < new_size; i++) {
            array->u.array.u.object[i] = zvm_null_object_ref;
        }
        break;
    case FUNCTION_INDEX_ARRAY:
    default:
        DBG_panic(("array->u.array.type..%d", array->u.array.type));
    }

    array->u.array.size = new_size;
}

void
ZVM_array_insert(ZVM_VirtualMachine *zvm, ZVM_Object *array, int pos,
                 ZVM_Value value)
{
    resize_array(zvm, array, array->u.array.size + 1);

    switch (array->u.array.type) {
    case INT_ARRAY:
        memmove(&array->u.array.u.int_array[pos+1],
                &array->u.array.u.int_array[pos],
                sizeof(int) * (array->u.array.size - pos));
        array->u.array.u.int_array[pos] = value.int_value;
        break;
    case DOUBLE_ARRAY:
        memmove(&array->u.array.u.double_array[pos+1],
                &array->u.array.u.double_array[pos],
                sizeof(double) * (array->u.array.size - pos));
        array->u.array.u.double_array[pos] = value.double_value;
        break;
    case OBJECT_ARRAY:
        memmove(&array->u.array.u.object[pos+1],
                &array->u.array.u.object[pos],
                sizeof(ZVM_ObjectRef) * (array->u.array.size - pos));
        array->u.array.u.object[pos] = value.object;
        break;
    case FUNCTION_INDEX_ARRAY:
    default:
        DBG_panic(("array->u.array.type..%d", array->u.array.type));
    }

    array->u.array.size++;
}

void
ZVM_array_remove(ZVM_VirtualMachine *zvm, ZVM_Object *array, int pos)
{

    switch (array->u.array.type) {
    case INT_ARRAY:
        memmove(&array->u.array.u.int_array[pos],
                &array->u.array.u.int_array[pos+1],
                sizeof(int) * (array->u.array.size - pos - 1));
        break;
    case DOUBLE_ARRAY:
        memmove(&array->u.array.u.double_array[pos],
                &array->u.array.u.double_array[pos+1],
                sizeof(double) * (array->u.array.size - pos - 1));
        break;
    case OBJECT_ARRAY:
        memmove(&array->u.array.u.object[pos],
                &array->u.array.u.object[pos+1],
                sizeof(ZVM_ObjectRef) * (array->u.array.size - pos - 1));
        break;
    case FUNCTION_INDEX_ARRAY:
    default:
        DBG_panic(("array->u.array.type..%d", array->u.array.type));
    }

    resize_array(zvm, array, array->u.array.size - 1);
    array->u.array.size--;
}

int
ZVM_string_length(ZVM_VirtualMachine *zvm, ZVM_Object *string)
{
    return zvm_wcslen(string->u.string.string);
}

ZVM_Value
ZVM_string_substr(ZVM_VirtualMachine *zvm, ZVM_Object *str,
                  int pos, int len)
{
    ZVM_Char *new_str;
    ZVM_Value ret;

    new_str = MEM_malloc(sizeof(ZVM_Char) * (len+1));
    zvm_wcsncpy(new_str, str->u.string.string + pos, len);
    new_str[len] = L'\0';
    ret.object = zvm_create_zvm_string_i(zvm, new_str);

    return ret;
}

static int
get_field_index_sub(ExecClass *ec, char *field_name, int *super_count)
{
    int i;
    int index;

    if (ec->super_class) {
        index = get_field_index_sub(ec->super_class, field_name, super_count);
        if (index != FIELD_NOT_FOUND) {
            return index;
        }
    }
    for (i = 0; i < ec->zvm_class->field_count; i++) {
        if (!strcmp(ec->zvm_class->field[i].name, field_name)) {
            return i + *super_count;
        }
    }
    *super_count += ec->zvm_class->field_count;

    return FIELD_NOT_FOUND;
}

int
ZVM_get_field_index(ZVM_VirtualMachine *zvm, ZVM_ObjectRef obj,
                    char *field_name)
{
    int super_count = 0;

    return get_field_index_sub(obj.v_table->exec_class, field_name,
                               &super_count);
}

void *
ZVM_object_get_native_pointer(ZVM_Object *obj)
{
    DBG_assert(obj->type == NATIVE_POINTER_OBJECT,
               ("obj->type..%d\n", obj->type));
    return obj->u.native_pointer.pointer;
}

ZVM_Boolean
ZVM_check_native_pointer_type(ZVM_Object *native_pointer,
                              ZVM_NativePointerInfo *info)
{
    return native_pointer->u.native_pointer.info == info;
}


void
ZVM_object_set_native_pointer(ZVM_Object *obj, void *p)
{
    DBG_assert(obj->type == NATIVE_POINTER_OBJECT,
               ("obj->type..%d\n", obj->type));
    obj->u.native_pointer.pointer = p;
}

void
ZVM_set_exception(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                  ZVM_NativeLibInfo *lib_info,
                  char *package_name, char *class_name,
                  int error_id, ...)
{
    int class_index;
    ZVM_ObjectRef obj;
    VString     message;
    va_list     ap;
    int message_index;
    int stack_trace_index;

    va_start(ap, error_id);
    class_index = zvm_search_class(zvm, package_name, class_name);
    obj = ZVM_create_class_object(zvm, context, class_index);

    zvm_format_message(lib_info->message_format, error_id,
                       &message, ap);
    va_end(ap);

    message_index
        = ZVM_get_field_index(zvm, obj, "message");
    obj.data->u.class_object.field[message_index].object
        = zvm_create_zvm_string_i(zvm, message.string);

    stack_trace_index
        = ZVM_get_field_index(zvm, obj, "stack_trace");
    obj.data->u.class_object.field[stack_trace_index].object
        = zvm_create_array_object_i(zvm, 0);

    zvm->current_exception = obj;
}
