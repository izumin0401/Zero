#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

void
ZVM_check_gc(ZVM_VirtualMachine *zvm)
{
    zvm_garbage_collect(zvm);
#if 0
    if (zvm->heap.current_heap_size > zvm->heap.current_threshold) {
        /* fprintf(stderr, "garbage collecting..."); */
        zvm_garbage_collect(zvm);
        /* fprintf(stderr, "done.\n"); */

        zvm->heap.current_threshold
            = zvm->heap.current_heap_size + HEAP_THRESHOLD_SIZE;
    }
#endif
}

static ZVM_ObjectRef
alloc_object(ZVM_VirtualMachine *zvm, ObjectType type)
{
    ZVM_ObjectRef ret;

    ZVM_check_gc(zvm);
    ret.v_table = NULL;
    ret.data = MEM_malloc(sizeof(ZVM_Object));
    zvm->heap.current_heap_size += sizeof(ZVM_Object);
    ret.data->type = type;
    ret.data->marked = ZVM_FALSE;
    ret.data->prev = NULL;
    ret.data->next = zvm->heap.header;
    zvm->heap.header = ret.data;
    if (ret.data->next) {
        ret.data->next->prev = ret.data;
    }

    return ret;
}

static void
add_ref_in_native_method(ZVM_Context *context, ZVM_ObjectRef *obj)
{
    RefInNativeFunc *new_ref;

    new_ref = MEM_malloc(sizeof(RefInNativeFunc));
    new_ref->object = *obj;
    new_ref->next = context->ref_in_native_method;
    context->ref_in_native_method = new_ref;
}

ZVM_ObjectRef
zvm_literal_to_zvm_string_i(ZVM_VirtualMachine *zvm, ZVM_Char *str)
{
    ZVM_ObjectRef ret;

    ret = alloc_object(zvm, STRING_OBJECT);
    ret.v_table = zvm->string_v_table;
    ret.data->u.string.string = str;
    ret.data->u.string.is_literal = ZVM_TRUE;

    return ret;
}

ZVM_ObjectRef
zvm_create_zvm_string_i(ZVM_VirtualMachine *zvm, ZVM_Char *str)
{
    ZVM_ObjectRef ret;

    ret = alloc_object(zvm, STRING_OBJECT);
    ret.v_table = zvm->string_v_table;
    ret.data->u.string.string = str;
    zvm->heap.current_heap_size += sizeof(ZVM_Char) * (zvm_wcslen(str) + 1);
    ret.data->u.string.is_literal = ZVM_FALSE;

    return ret;
}

ZVM_ObjectRef
ZVM_create_zvm_string(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                      ZVM_Char *str)
{
    ZVM_ObjectRef ret;

    ret = zvm_create_zvm_string_i(zvm, str);
    add_ref_in_native_method(context, &ret);

    return ret;
}

ZVM_ObjectRef
alloc_array(ZVM_VirtualMachine *zvm, ArrayType type, int size)
{
    ZVM_ObjectRef ret;

    ret = alloc_object(zvm, ARRAY_OBJECT);
    ret.data->u.array.type = type;
    ret.data->u.array.size = size;
    ret.data->u.array.alloc_size = size;
    ret.v_table = zvm->array_v_table;

    return ret;
}

ZVM_ObjectRef
zvm_create_array_int_i(ZVM_VirtualMachine *zvm, int size)
{
    ZVM_ObjectRef ret;
    int i;

    ret = alloc_array(zvm, INT_ARRAY, size);
    ret.data->u.array.u.int_array = MEM_malloc(sizeof(int) * size);
    zvm->heap.current_heap_size += sizeof(int) * size;
    for (i = 0; i < size; i++) {
        ret.data->u.array.u.int_array[i] = 0;
    }

    return ret;
}

ZVM_ObjectRef
ZVM_create_array_int(ZVM_VirtualMachine *zvm,  ZVM_Context *context,
                     int size)
{
    ZVM_ObjectRef ret;

    ret = zvm_create_array_int_i(zvm, size);

    return ret;
}

ZVM_ObjectRef
zvm_create_array_double_i(ZVM_VirtualMachine *zvm, int size)
{
    ZVM_ObjectRef ret;
    int i;

    ret = alloc_array(zvm, DOUBLE_ARRAY, size);
    ret.data->u.array.u.double_array = MEM_malloc(sizeof(double) * size);
    zvm->heap.current_heap_size += sizeof(double) * size;
    for (i = 0; i < size; i++) {
        ret.data->u.array.u.double_array[i] = 0.0;
    }

    return ret;
}

ZVM_ObjectRef
ZVM_create_array_double(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                        int size)
{
    ZVM_ObjectRef ret;

    ret = zvm_create_array_double_i(zvm, size);
    add_ref_in_native_method(context, &ret);

    return ret;
}

ZVM_ObjectRef
zvm_create_array_object_i(ZVM_VirtualMachine *zvm, int size)
{
    ZVM_ObjectRef ret;
    int i;

    ret = alloc_array(zvm, OBJECT_ARRAY, size);
    ret.data->u.array.u.object = MEM_malloc(sizeof(ZVM_ObjectRef) * size);
    zvm->heap.current_heap_size += sizeof(ZVM_ObjectRef) * size;
    for (i = 0; i < size; i++) {
        ret.data->u.array.u.object[i] = zvm_null_object_ref;
    }

    return ret;
}

ZVM_ObjectRef
ZVM_create_array_object(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                        int size)
{
    ZVM_ObjectRef ret;

    ret = zvm_create_array_object_i(zvm, size);
    add_ref_in_native_method(context, &ret);

    return ret;
}

ZVM_ObjectRef
zvm_create_class_object_i(ZVM_VirtualMachine *zvm, int class_index)
{
    ExecClass *ec;
    ZVM_ObjectRef obj;
    int i;

    obj = alloc_object(zvm, CLASS_OBJECT);

    ec = zvm->class[class_index];
    
    obj.v_table = ec->class_table;

    obj.data->u.class_object.field_count = ec->field_count;
    obj.data->u.class_object.field
        = MEM_malloc(sizeof(ZVM_Value) * ec->field_count);
    for (i = 0; i < ec->field_count; i++) {
        zvm_initialize_value(ec->field_type[i],
                             &obj.data->u.class_object.field[i]);
    }

    return obj;
}

ZVM_ObjectRef
ZVM_create_class_object(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                        int class_index)
{
    ZVM_ObjectRef ret;

    ret = zvm_create_class_object_i(zvm, class_index);
    add_ref_in_native_method(context, &ret);

    return ret;
}

static ZVM_ObjectRef
create_native_pointer_i(ZVM_VirtualMachine *zvm, void *pointer,
                            ZVM_NativePointerInfo *info)
{
    ZVM_ObjectRef ret;

    ret = alloc_object(zvm, NATIVE_POINTER_OBJECT);
    ret.data->u.native_pointer.pointer = pointer;
    ret.data->u.native_pointer.info = info;
    ret.v_table = NULL;

    return ret;
}

ZVM_ObjectRef
ZVM_create_native_pointer(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                          void *pointer, ZVM_NativePointerInfo *info)
{
    ZVM_ObjectRef ret;

    ret = create_native_pointer_i(zvm, pointer, info);
    add_ref_in_native_method(context, &ret);

    return ret;
}

ZVM_ObjectRef
zvm_create_delegate(ZVM_VirtualMachine *zvm, ZVM_ObjectRef object,
                    int index)
{
    ZVM_ObjectRef ret;

    ret = alloc_object(zvm, DELEGATE_OBJECT);
    ret.data->u.delegate.object = object;
    ret.data->u.delegate.index = index;
    ret.v_table = NULL;

    return ret;
}

static ZVM_Boolean
is_reference_type(ZVM_TypeSpecifier *type)
{
    if (((type->basic_type == ZVM_STRING_TYPE
          || type->basic_type == ZVM_CLASS_TYPE
          || type->basic_type == ZVM_DELEGATE_TYPE
          || type->basic_type == ZVM_NATIVE_POINTER_TYPE)
         && type->derive_count == 0)
        || (type->derive_count > 0
            && type->derive[0].tag == ZVM_ARRAY_DERIVE)) {
        return ZVM_TRUE;
    }
    return ZVM_FALSE;
}

static void
gc_mark(ZVM_ObjectRef *ref)
{
    int i;

    if (ref->data == NULL)
        return;

    if (ref->data->marked)
        return;

    ref->data->marked = ZVM_TRUE;

    if (ref->data->type == ARRAY_OBJECT
        && ref->data->u.array.type == OBJECT_ARRAY) {
        for (i = 0; i < ref->data->u.array.size; i++) {
            gc_mark(&ref->data->u.array.u.object[i]);
        }
    } else if (ref->data->type == CLASS_OBJECT) {
        ExecClass *ec = ref->v_table->exec_class;
        for (i = 0; i < ec->field_count; i++) {
#if 0
            if (ec->field_type[i]->basic_type == ZVM_STRING_TYPE
                || ec->field_type[i]->basic_type == ZVM_CLASS_TYPE
                || (ec->field_type[i]->derive_count > 0
                    && ec->field_type[i]->derive[0].tag == ZVM_ARRAY_DERIVE)) {
#endif
            if (is_reference_type(ec->field_type[i])) {
                gc_mark(&ref->data->u.class_object.field[i].object);
            }
        }
    } else if (ref->data->type == DELEGATE_OBJECT) {
        gc_mark(&ref->data->u.delegate.object);
    }
}

static void
gc_mark_ref_in_native_method(ZVM_Context *context)
{
    RefInNativeFunc *ref;

    if (context == NULL)
        return;

    for (ref = context->ref_in_native_method; ref; ref = ref->next) {
        gc_mark(&ref->object);
    }
}

static void
gc_reset_mark(ZVM_Object *obj)
{
    obj->marked = ZVM_FALSE;
}

static void
gc_mark_objects(ZVM_VirtualMachine *zvm)
{
    ZVM_Object *obj;
    ExecutableEntry *ee_pos;
    int i;

    for (obj = zvm->heap.header; obj; obj = obj->next) {
        gc_reset_mark(obj);
    }

    for (ee_pos = zvm->executable_entry; ee_pos; ee_pos = ee_pos->next) {
        for (i = 0; i < ee_pos->static_v.variable_count; i++) {
            /*
            if (((ee_pos->executable->global_variable[i].type->basic_type
                  == ZVM_STRING_TYPE
                  || ee_pos->executable->global_variable[i].type->basic_type
                  == ZVM_CLASS_TYPE
                  || ee_pos->executable->global_variable[i].type->basic_type
                  == ZVM_DELEGATE_TYPE
                  || ee_pos->executable->global_variable[i].type->basic_type
                  == ZVM_NATIVE_POINTER_TYPE)
                 && (ee_pos->executable->global_variable[i].type->derive_count
                     == 0))
                || (ee_pos->executable->global_variable[i].type->derive != NULL
                    && (ee_pos->executable->global_variable[i].type
                        ->derive[0].tag == ZVM_ARRAY_DERIVE))) {
            */
            if (is_reference_type(ee_pos->executable
                                  ->global_variable[i].type)) {
                gc_mark(&ee_pos->static_v.variable[i].object);
            }
        }
    }

    for (i = 0; i < zvm->stack.stack_pointer; i++) {
        if (zvm->stack.pointer_flags[i]) {
            gc_mark(&zvm->stack.stack[i].object);
        }
    }
    gc_mark(&zvm->current_exception);
    gc_mark_ref_in_native_method(zvm->current_context);
}

static void
gc_dispose_object(ZVM_VirtualMachine *zvm, ZVM_Object *obj)
{
    switch (obj->type) {
    case STRING_OBJECT:
        if (!obj->u.string.is_literal) {
            zvm->heap.current_heap_size
                -= sizeof(ZVM_Char) * (zvm_wcslen(obj->u.string.string) + 1);
            MEM_free(obj->u.string.string);
        }
        break;
    case ARRAY_OBJECT:
        switch (obj->u.array.type) {
        case INT_ARRAY:
            zvm->heap.current_heap_size
                -= sizeof(int) * obj->u.array.alloc_size;
            MEM_free(obj->u.array.u.int_array);
            break;
        case DOUBLE_ARRAY:
            zvm->heap.current_heap_size
                -= sizeof(double) * obj->u.array.alloc_size;
            MEM_free(obj->u.array.u.double_array);
            break;
        case OBJECT_ARRAY:
            zvm->heap.current_heap_size
                -= sizeof(ZVM_Object*) * obj->u.array.alloc_size;
            MEM_free(obj->u.array.u.object);
            break;
        case FUNCTION_INDEX_ARRAY:
            zvm->heap.current_heap_size
                -= sizeof(int) * obj->u.array.alloc_size;
            MEM_free(obj->u.array.u.function_index);
        default:
            DBG_assert(0, ("array.type..%d", obj->u.array.type));
        }
        break;
    case CLASS_OBJECT:
        zvm->heap.current_heap_size
            -= sizeof(ZVM_Value) * obj->u.class_object.field_count;
        MEM_free(obj->u.class_object.field);
        break;
    case NATIVE_POINTER_OBJECT:
        if (obj->u.native_pointer.info->finalizer) {
            obj->u.native_pointer.info->finalizer(zvm, obj);
        }
        break;
    case DELEGATE_OBJECT:
        break;
    case OBJECT_TYPE_COUNT_PLUS_1:
    default:
        DBG_assert(0, ("bad type..%d\n", obj->type));
    }
    zvm->heap.current_heap_size -= sizeof(ZVM_Object);
    MEM_free(obj);
}

static void
gc_sweep_objects(ZVM_VirtualMachine *zvm)
{
    ZVM_Object *obj;
    ZVM_Object *tmp;

    for (obj = zvm->heap.header; obj; ) {
        if (!obj->marked) {
            if (obj->prev) {
                obj->prev->next = obj->next;
            } else {
                zvm->heap.header = obj->next;
            }
            if (obj->next) {
                obj->next->prev = obj->prev;
            }
            tmp = obj->next;
            gc_dispose_object(zvm, obj);
            obj = tmp;
        } else {
            obj = obj->next;
        }
    }
}

void
zvm_garbage_collect(ZVM_VirtualMachine *zvm)
{
    gc_mark_objects(zvm);
    gc_sweep_objects(zvm);
}
