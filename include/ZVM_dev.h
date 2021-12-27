#ifndef ZVM_DEV_H_INCLUDED
#define ZVM_DEV_H_INCLUDED
#include "ZVM.h"

typedef struct ZVM_Array_tag ZVM_Array;
typedef struct ZVM_Context_tag ZVM_Context;

typedef enum {
    ZVM_SUCCESS = 1,
    ZVM_ERROR
} ZVM_ErrorStatus;

typedef ZVM_Value ZVM_NativeFunctionProc(ZVM_VirtualMachine *zvm,
                                         ZVM_Context *context,
                                         int arg_count, ZVM_Value *args);

typedef void ZVM_NativePointerFinalizeProc(ZVM_VirtualMachine *zvm,
                                           ZVM_Object *obj);

typedef struct {
    char                                *name;
    ZVM_NativePointerFinalizeProc       *finalizer;
} ZVM_NativePointerInfo;

typedef enum {
    ZVM_INT_MESSAGE_ARGUMENT = 1,
    ZVM_DOUBLE_MESSAGE_ARGUMENT,
    ZVM_STRING_MESSAGE_ARGUMENT,
    ZVM_CHARACTER_MESSAGE_ARGUMENT,
    ZVM_POINTER_MESSAGE_ARGUMENT,
    ZVM_MESSAGE_ARGUMENT_END
} ZVM_MessageArgumentType;

typedef struct {
    char *format;
} ZVM_ErrorDefinition;

typedef struct {
    ZVM_ErrorDefinition *message_format;
} ZVM_NativeLibInfo;

/* execute.c */
void ZVM_add_native_function(ZVM_VirtualMachine *zvm,
                             char *package_name, char *func_name,
                             ZVM_NativeFunctionProc *proc, int arg_count,
                             ZVM_Boolean is_method,
                             ZVM_Boolean return_pointer);
/* nativeif.c */
ZVM_ErrorStatus
ZVM_array_get_int(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                  int *value, ZVM_ObjectRef *exception_p);
ZVM_ErrorStatus
ZVM_array_get_double(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array,
                     int index, double *value,
                     ZVM_ObjectRef *exception_p);
ZVM_ErrorStatus
ZVM_array_get_object(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                     ZVM_ObjectRef *value, ZVM_ObjectRef *exception_p);
ZVM_ErrorStatus
ZVM_array_set_int(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                  int value, ZVM_ObjectRef *exception_p);
ZVM_ErrorStatus
ZVM_array_set_double(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                     double value, ZVM_ObjectRef *exception_p);
ZVM_ErrorStatus
ZVM_array_set_object(ZVM_VirtualMachine *zvm, ZVM_ObjectRef array, int index,
                     ZVM_ObjectRef value, ZVM_ObjectRef *exception_p);
int ZVM_array_size(ZVM_VirtualMachine *zvm, ZVM_Object *array);
void
ZVM_array_resize(ZVM_VirtualMachine *zvm, ZVM_Object *array, int new_size);
void ZVM_array_insert(ZVM_VirtualMachine *zvm, ZVM_Object *array, int pos,
                      ZVM_Value value);
void ZVM_array_remove(ZVM_VirtualMachine *zvm, ZVM_Object *array, int pos);
int ZVM_string_length(ZVM_VirtualMachine *zvm, ZVM_Object *string);
ZVM_Value ZVM_string_substr(ZVM_VirtualMachine *zvm, ZVM_Object *str,
                            int pos, int len);
int ZVM_get_field_index(ZVM_VirtualMachine *zvm, ZVM_ObjectRef obj,
                        char *field_name);
void ZVM_set_exception(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                       ZVM_NativeLibInfo *lib_info,
                       char *package_name, char *class_name,
                       int error_id, ...);

/* heap.c */
void ZVM_check_gc(ZVM_VirtualMachine *zvm);
ZVM_ObjectRef
ZVM_create_array_int(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                     int size);
ZVM_ObjectRef
ZVM_create_array_double(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                        int size);
ZVM_ObjectRef
ZVM_create_array_object(ZVM_VirtualMachine *zvm,  ZVM_Context *context,
                        int size);
void *ZVM_object_get_native_pointer(ZVM_Object *obj);
ZVM_Boolean ZVM_check_native_pointer_type(ZVM_Object *native_pointer,
                                          ZVM_NativePointerInfo *info);
void ZVM_object_set_native_pointer(ZVM_Object *obj, void *p);
ZVM_ObjectRef
ZVM_create_zvm_string(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                      ZVM_Char *str);
ZVM_ObjectRef
ZVM_create_class_object(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                        int class_index);
ZVM_ObjectRef
ZVM_create_native_pointer(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                          void *pointer, ZVM_NativePointerInfo *info);

/* wchar.c */
char *ZVM_wcstombs(const wchar_t *src);
ZVM_Char *ZVM_mbstowcs(const char *src);

#endif /* ZVM_DEV_H_INCLUDED */
