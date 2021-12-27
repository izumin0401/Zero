#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zvm_pri.h"

static void file_finalizer(ZVM_VirtualMachine *zvm, ZVM_Object* obj);

static ZVM_NativePointerInfo st_file_type_info = {
    "zero.lang.file_pointer",
    file_finalizer
};

extern ZVM_ErrorDefinition zvm_native_error_message_format[];

static ZVM_NativeLibInfo st_lib_info = {
    zvm_native_error_message_format,
};

typedef enum {
    INSERT_INDEX_OUT_OF_BOUNDS_ERR,
    REMOVE_INDEX_OUT_OF_BOUNDS_ERR,
    STRING_POS_OUT_OF_BOUNDS_ERR,
    STRING_SUBSTR_LEN_ERR,
    FOPEN_1ST_ARG_NULL_ERR,
    FOPEN_2ND_ARG_NULL_ERR,
    FGETS_ARG_NULL_ERR,
    FGETS_FP_BAD_TYPE_ERR,
    FGETS_FP_INVALID_ERR,
    FGETS_BAD_MULTIBYTE_CHARACTER_ERR,
    FPUTS_2ND_ARG_NULL_ERR,
    FPUTS_FP_BAD_TYPE_ERR,
    FPUTS_FP_INVALID_ERR,
    FCLOSE_ARG_NULL_ERR,
    FCLOSE_FP_BAD_TYPE_ERR,
    FCLOSE_FP_INVALID_ERR,
    NATIVE_RUNTIME_ERROR_COUNT_PLUS_1
} NativeRuntimeError;

static void
file_finalizer(ZVM_VirtualMachine *zvm, ZVM_Object *obj)
{
    FILE *fp;

    fp = (FILE*)ZVM_object_get_native_pointer(obj);
    if (fp && fp != stdin && fp != stdout && fp != stderr) {
        fclose(fp);
    }
}

static ZVM_Value
nv_print_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
              int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Char *str;

    ret.int_value = 0;

    DBG_assert(arg_count == 1, ("arg_count..%d", arg_count));

    if (args[0].object.data == NULL) {
        str = NULL_STRING;
    } else {
        str = args[0].object.data->u.string.string;
    }
    zvm_print_wcs(stdout, str);
    fflush(stdout);

    return ret;
}

static ZVM_Value
nv_fopen_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
              int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    char *file_name;
    char *mode;
    FILE *fp;

    DBG_assert(arg_count == 2, ("arg_count..%d", arg_count));

    if (is_object_null(args[0].object)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FOPEN_1ST_ARG_NULL_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    if (is_object_null(args[1].object)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FOPEN_2ND_ARG_NULL_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    file_name = ZVM_wcstombs(args[0].object.data->u.string.string);
    mode = ZVM_wcstombs(args[1].object.data->u.string.string);
    fp = fopen(file_name, mode);
    MEM_free(file_name);
    MEM_free(mode);

    ret.object = ZVM_create_native_pointer(zvm, context, fp,
                                           &st_file_type_info);

    return ret;
}

static ZVM_Value
nv_fgets_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
              int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    FILE *fp;
    char buf[LINE_BUF_SIZE];
    char *mb_buf = NULL;
    int ret_len = 0;
    ZVM_Char *wc_str;

    DBG_assert(arg_count == 1, ("arg_count..%d", arg_count));

    if (is_object_null(args[0].object)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FOPEN_1ST_ARG_NULL_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    if (!ZVM_check_native_pointer_type(args[0].object.data,
                                       &st_file_type_info)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FGETS_FP_BAD_TYPE_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    fp = ZVM_object_get_native_pointer(args[0].object.data);
    if (fp == NULL) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FGETS_FP_INVALID_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }

    while (fgets(buf, LINE_BUF_SIZE, fp)) {
        int new_len;
        new_len = ret_len + strlen(buf);
        mb_buf = MEM_realloc(mb_buf, new_len + 1);
        if (ret_len == 0) {
            strcpy(mb_buf, buf);
        } else {
            strcat(mb_buf, buf);
        }
        ret_len = new_len;
        if (mb_buf[ret_len-1] == '\n')
            break;
    }
    if (ret_len > 0) {
        wc_str = ZVM_mbstowcs(mb_buf);
        if (wc_str == NULL) {
            MEM_free(mb_buf);
            ZVM_set_exception(zvm, context, &st_lib_info,
                              ZERO_DEFAULT_PACKAGE,
                              MULTIBYTE_CONVERTION_EXCEPTION_NAME,
                              (int)FGETS_BAD_MULTIBYTE_CHARACTER_ERR,
                              ZVM_MESSAGE_ARGUMENT_END);
        }
        ret.object = ZVM_create_zvm_string(zvm, context, wc_str);
    } else {
        ret.object = zvm_null_object_ref;
    }
    MEM_free(mb_buf);

    return ret;
}

static ZVM_Value
nv_fputs_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
              int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Char *str;
    FILE *fp;

    ret.int_value = 0;

    DBG_assert(arg_count == 2, ("arg_count..%d", arg_count));

    if (args[0].object.data == NULL) {
        str = NULL_STRING;
    } else {
        str = args[0].object.data->u.string.string;
    }
    if (is_object_null(args[1].object)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FPUTS_2ND_ARG_NULL_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    if (!ZVM_check_native_pointer_type(args[1].object.data,
                                       &st_file_type_info)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FPUTS_FP_BAD_TYPE_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    fp = ZVM_object_get_native_pointer(args[1].object.data);
    if (fp == NULL) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FPUTS_FP_INVALID_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    zvm_print_wcs(fp, str);

    return ret;
}

static ZVM_Value
nv_fclose_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
               int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    FILE *fp;

    ret.int_value = 0;

    DBG_assert(arg_count == 1, ("arg_count..%d", arg_count));

    if (is_object_null(args[0].object)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FCLOSE_ARG_NULL_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    if (!ZVM_check_native_pointer_type(args[0].object.data,
                                       &st_file_type_info)) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FCLOSE_FP_BAD_TYPE_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    fp = ZVM_object_get_native_pointer(args[0].object.data);
    if (fp == NULL) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, NULL_POINTER_EXCEPTION_NAME,
                          FCLOSE_FP_INVALID_ERR,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    fclose(fp);
    ZVM_object_set_native_pointer(args[0].object.data, NULL);

    return ret;
}

static ZVM_Value
nv_array_size_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                   int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Object *array;

    DBG_assert(arg_count == 0, ("arg_count..%d", arg_count));

    array = args[0].object.data;
    DBG_assert(array->type == ARRAY_OBJECT, ("array->type..%d", array->type));

    ret.int_value = ZVM_array_size(zvm, array);

    return ret;
}

static ZVM_Value
nv_array_resize_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                     int arg_count, ZVM_Value *args)
{
    ZVM_Value ret; /* dummy */
    ZVM_Object *array;
    int new_size;

    DBG_assert(arg_count == 1, ("arg_count..%d", arg_count));

    new_size = args[0].int_value;
    array = args[1].object.data;
    DBG_assert(array->type == ARRAY_OBJECT, ("array->type..%d", array->type));

    ZVM_array_resize(zvm, array, new_size);

    return ret;
}

static ZVM_Value
nv_array_insert_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                     int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Object *array;
    ZVM_Value value;
    int pos;
    int array_size;

    pos = args[0].int_value;
    value = args[1];
    array = args[2].object.data;

    array_size = ZVM_array_size(zvm, array);

    if (pos < 0 || pos > array_size) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, ARRAY_INDEX_EXCEPTION_NAME,
                          INSERT_INDEX_OUT_OF_BOUNDS_ERR,
                          ZVM_INT_MESSAGE_ARGUMENT, "pos", pos,
                          ZVM_INT_MESSAGE_ARGUMENT, "size", array_size,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    ZVM_array_insert(zvm, array, pos, value);

    return ret;
}

ZVM_Value
nv_array_remove_proc(ZVM_VirtualMachine *zvm,  ZVM_Context *context,
                     int arg_count, ZVM_Value *args)
{
    ZVM_Value ret; /* dummy */
    ZVM_Object *array;
    int pos;
    int array_size;

    DBG_assert(arg_count == 1, ("arg_count..%d", arg_count));

    pos = args[0].int_value;
    array = args[1].object.data;
    DBG_assert(array->type == ARRAY_OBJECT, ("array->type..%d", array->type));

    array_size = ZVM_array_size(zvm, array);
    if (pos < 0 || pos >= array_size) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, ARRAY_INDEX_EXCEPTION_NAME,
                          REMOVE_INDEX_OUT_OF_BOUNDS_ERR,
                          ZVM_INT_MESSAGE_ARGUMENT, "pos", pos,
                          ZVM_INT_MESSAGE_ARGUMENT, "size", array_size,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    ZVM_array_remove(zvm, array, pos);

    return ret;
}

static ZVM_Value
nv_array_add_proc(ZVM_VirtualMachine *zvm,  ZVM_Context *context,
                  int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Object *array;
    ZVM_Value value;
    int array_size;

    DBG_assert(arg_count == 1, ("arg_count..%d", arg_count));
    value = args[0];
    array = args[1].object.data;

    array_size = ZVM_array_size(zvm, array);

    ZVM_array_insert(zvm, array, array_size, value);

    return ret;
}

static ZVM_Value
nv_string_length_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                      int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Object *str;

    DBG_assert(arg_count == 0, ("arg_count..%d", arg_count));

    str = args[0].object.data;
    DBG_assert(str->type == STRING_OBJECT,
               ("str->type..%d", str->type));

    ret.int_value = ZVM_string_length(zvm, str);

    return ret;
}

static ZVM_Value
nv_string_substr_proc(ZVM_VirtualMachine *zvm, ZVM_Context *context,
                      int arg_count, ZVM_Value *args)
{
    ZVM_Value ret;
    ZVM_Object *str;
    int pos;
    int len;
    int org_len;

    DBG_assert(arg_count == 2, ("arg_count..%d", arg_count));

    pos = args[0].int_value;
    len = args[1].int_value;
    str = args[2].object.data;
    DBG_assert(str->type == STRING_OBJECT,
               ("str->type..%d", str->type));

    org_len = ZVM_string_length(zvm, str);
    
    if (pos < 0 || pos >= org_len) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, STRING_INDEX_EXCEPTION_NAME,
                          STRING_POS_OUT_OF_BOUNDS_ERR,
                          ZVM_INT_MESSAGE_ARGUMENT, "len", org_len,
                          ZVM_INT_MESSAGE_ARGUMENT, "pos", pos,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }
    if (len < 0 || pos + len > org_len) {
        ZVM_set_exception(zvm, context, &st_lib_info,
                          ZERO_DEFAULT_PACKAGE, STRING_INDEX_EXCEPTION_NAME,
                          STRING_SUBSTR_LEN_ERR,
                          ZVM_INT_MESSAGE_ARGUMENT, "len", len,
                          ZVM_MESSAGE_ARGUMENT_END);
        return ret;
    }

    ret = ZVM_string_substr(zvm, str, pos, len);

    return ret;
}

void
zvm_add_native_functions(ZVM_VirtualMachine *zvm)
{
    ZVM_add_native_function(zvm, "zero.lang", "print", nv_print_proc, 1,
                            ZVM_FALSE, ZVM_FALSE);
    ZVM_add_native_function(zvm, "zero.lang", "__fopen", nv_fopen_proc, 2,
                            ZVM_FALSE, ZVM_TRUE);
    ZVM_add_native_function(zvm, "zero.lang", "__fgets", nv_fgets_proc, 1,
                            ZVM_FALSE, ZVM_TRUE);
    ZVM_add_native_function(zvm, "zero.lang", "__fputs", nv_fputs_proc, 2,
                            ZVM_FALSE, ZVM_FALSE);
    ZVM_add_native_function(zvm, "zero.lang", "__fclose", nv_fclose_proc, 1,
                            ZVM_FALSE, ZVM_FALSE);

    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            ARRAY_PREFIX ARRAY_METHOD_SIZE,
                            nv_array_size_proc, 0, ZVM_TRUE, ZVM_FALSE);
    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            ARRAY_PREFIX ARRAY_METHOD_RESIZE,
                            nv_array_resize_proc, 1, ZVM_TRUE, ZVM_FALSE);
    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            ARRAY_PREFIX ARRAY_METHOD_INSERT,
                            nv_array_insert_proc, 2, ZVM_TRUE, ZVM_FALSE);
    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            ARRAY_PREFIX ARRAY_METHOD_REMOVE,
                            nv_array_remove_proc, 1, ZVM_TRUE, ZVM_FALSE);
    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            ARRAY_PREFIX ARRAY_METHOD_ADD,
                            nv_array_add_proc, 1, ZVM_TRUE, ZVM_FALSE);

    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            STRING_PREFIX STRING_METHOD_LENGTH,
                            nv_string_length_proc, 0, ZVM_TRUE, ZVM_FALSE);
    ZVM_add_native_function(zvm, BUILT_IN_METHOD_PACKAGE_NAME,
                            STRING_PREFIX STRING_METHOD_SUBSTR,
                            nv_string_substr_proc, 2, ZVM_TRUE, ZVM_FALSE);
}
