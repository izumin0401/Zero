#ifndef ZVM_PRI_H_INCLUDED
#define ZVM_PRI_H_INCLUDED
#include "ZVM_code.h"
#include "ZVM_dev.h"
#include "share.h"
#include <stdarg.h>

#define STACK_ALLOC_SIZE (4096)
#define HEAP_THRESHOLD_SIZE     (1024 * 256)
#define ARRAY_ALLOC_SIZE (256)
#define NULL_STRING (L"null")
#define TRUE_STRING (L"true")
#define FALSE_STRING (L"false")
#define BUILT_IN_METHOD_PACKAGE_NAME ("$built_in")

#define NO_LINE_NUMBER_PC (-1)
#define FUNCTION_NOT_FOUND (-1)
#define FIELD_NOT_FOUND (-1)
#define CALL_FROM_NATIVE (-1)

#define MESSAGE_ARGUMENT_MAX    (256)
#define LINE_BUF_SIZE (1024)

#define GET_2BYTE_INT(p) (((p)[0] << 8) + (p)[1])
#define SET_2BYTE_INT(p, value) \
  (((p)[0] = (value) >> 8), ((p)[1] = value & 0xff))

#define is_pointer_type(type) \
  ((type)->basic_type == ZVM_STRING_TYPE \
   || (type)->basic_type == ZVM_CLASS_TYPE \
   || (type)->basic_type == ZVM_NULL_TYPE \
   || (type)->basic_type == ZVM_DELEGATE_TYPE \
   || ((type)->derive_count > 0 \
       && (type)->derive[0].tag == ZVM_ARRAY_DERIVE))

#define is_object_null(obj) ((obj).data == NULL)

typedef enum {
    BAD_MULTIBYTE_CHARACTER_ERR = 1,
    FUNCTION_NOT_FOUND_ERR,
    FUNCTION_MULTIPLE_DEFINE_ERR,
    INDEX_OUT_OF_BOUNDS_ERR,
    DIVISION_BY_ZERO_ERR,
    NULL_POINTER_ERR,
    LOAD_FILE_NOT_FOUND_ERR,
    LOAD_FILE_ERR,
    CLASS_MULTIPLE_DEFINE_ERR,
    CLASS_NOT_FOUND_ERR,
    CLASS_CAST_ERR,
    RUNTIME_ERROR_COUNT_PLUS_1
} RuntimeError;

typedef struct {
    ZVM_Char    *string;
} VString;

typedef enum {
    NATIVE_FUNCTION,
    ZERO_FUNCTION
} FunctionKind;

typedef struct {
    ZVM_NativeFunctionProc *proc;
    int arg_count;
    ZVM_Boolean is_method;
    ZVM_Boolean return_pointer;
} NativeFunction;

typedef struct ExecutableEntry_tag ExecutableEntry;

typedef struct {
    ExecutableEntry     *executable;
    int                 index;
} ZeroFunction;

typedef struct {
    char                *package_name;
    char                *name;
    FunctionKind        kind;
    ZVM_Boolean         is_implemented;
    union {
        NativeFunction native_f;
        ZeroFunction zero_f;
    } u;
} Function;

typedef struct {
    Function    *caller;
    int         caller_address;
    int         base;
} CallInfo;

#define revalue_up_align(val)   ((val) ? (((val) - 1) / sizeof(ZVM_Value) + 1)\
                                 : 0)
#define CALL_INFO_ALIGN_SIZE    (revalue_up_align(sizeof(CallInfo)))

typedef union {
    CallInfo    s;
    ZVM_Value   u[CALL_INFO_ALIGN_SIZE];
} CallInfoUnion;

typedef struct {
    int         alloc_size;
    int         stack_pointer;
    ZVM_Value   *stack;
    ZVM_Boolean *pointer_flags;
} Stack;

typedef enum {
    STRING_OBJECT = 1,
    ARRAY_OBJECT,
    CLASS_OBJECT,
    NATIVE_POINTER_OBJECT,
    DELEGATE_OBJECT,
    OBJECT_TYPE_COUNT_PLUS_1
} ObjectType;

struct ZVM_String_tag {
    ZVM_Boolean is_literal;
    ZVM_Char    *string;
};

typedef enum {
    INT_ARRAY = 1,
    DOUBLE_ARRAY,
    OBJECT_ARRAY,
    FUNCTION_INDEX_ARRAY
} ArrayType;

struct ZVM_Array_tag {
    ArrayType   type;
    int         size;
    int         alloc_size;
    union {
        int             *int_array;
        double          *double_array;
        ZVM_ObjectRef   *object;
        int             *function_index;
    } u;
};

typedef struct {
    int         field_count;
    ZVM_Value   *field;
} ZVM_ClassObject;

typedef struct {
    void                        *pointer;
    ZVM_NativePointerInfo       *info;
} NativePointer;

typedef struct {
    ZVM_ObjectRef       object;
    int                 index; /* if object is null, function index,
                                  else, method index*/
} Delegate;

struct ZVM_Object_tag {
    ObjectType  type;
    unsigned int        marked:1;
    union {
        ZVM_String      string;
        ZVM_Array       array;
        ZVM_ClassObject class_object;
        NativePointer   native_pointer;
        Delegate        delegate;
    } u;
    struct ZVM_Object_tag *prev;
    struct ZVM_Object_tag *next;
};

typedef struct {
    int         current_heap_size;
    int         current_threshold;
    ZVM_Object  *header;
} Heap;

typedef struct {
    int         variable_count;
    ZVM_Value   *variable;
} Static;

typedef struct ExecClass_tag {
    ZVM_Class           *zvm_class;
    char                *package_name;
    char                *name;
    ZVM_Boolean         is_implemented;
    int                 class_index;
    struct ExecClass_tag *super_class;
    ZVM_VTable          *class_table;
    int                 interface_count;
    struct ExecClass_tag **interface;
    ZVM_VTable          **interface_v_table;
    int                 field_count;
    ZVM_TypeSpecifier   **field_type;
} ExecClass;

typedef struct {
    char        *name;
    int         index;
} VTableItem;

struct ZVM_VTable_tag {
    ExecClass   *exec_class;
    int         table_size;
    VTableItem  *table;
};

struct ExecutableEntry_tag {
    ZVM_Executable *executable;
    int         *function_table;
    int         *class_table;
    Static      static_v;
    struct ExecutableEntry_tag *next;
};

struct ZVM_VirtualMachine_tag {
    Stack       stack;
    Heap        heap;
    ExecutableEntry     *current_executable;
    Function    *current_function;
    ZVM_ObjectRef current_exception;
    int         pc;
    Function    **function;
    int         function_count;
    ExecClass   **class;
    int         class_count;
    ZVM_ExecutableList  *executable_list;
    ExecutableEntry     *executable_entry;
    ExecutableEntry     *top_level;
    ZVM_VTable  *array_v_table;
    ZVM_VTable  *string_v_table;
    ZVM_Context *current_context;
};

typedef struct RefInNativeFunc_tag {
    ZVM_ObjectRef  object;
    struct RefInNativeFunc_tag *next;
} RefInNativeFunc;

struct ZVM_Context_tag {
    RefInNativeFunc     *ref_in_native_method;
};

/* execute.c */
ZVM_ObjectRef
zvm_create_exception(ZVM_VirtualMachine *zvm, char *class_name,
                     RuntimeError id, ...);

/* heap.c */
ZVM_ObjectRef
zvm_literal_to_zvm_string_i(ZVM_VirtualMachine *inter, ZVM_Char *str);
ZVM_ObjectRef
zvm_create_zvm_string_i(ZVM_VirtualMachine *zvm, ZVM_Char *str);
ZVM_ObjectRef zvm_create_array_int_i(ZVM_VirtualMachine *zvm, int size);
ZVM_ObjectRef zvm_create_array_double_i(ZVM_VirtualMachine *zvm, int size);
ZVM_ObjectRef zvm_create_array_object_i(ZVM_VirtualMachine *zvm, int size);
ZVM_ObjectRef zvm_create_class_object_i(ZVM_VirtualMachine *zvm,
                                        int class_index);
ZVM_ObjectRef zvm_create_delegate(ZVM_VirtualMachine *zvm,
                                  ZVM_ObjectRef object, int index);
void zvm_garbage_collect(ZVM_VirtualMachine *zvm);
/* native.c */
void zvm_add_native_functions(ZVM_VirtualMachine *zvm);
/* load.c*/
int zvm_search_class(ZVM_VirtualMachine *zvm, char *package_name, char *name);
int zvm_search_function(ZVM_VirtualMachine *zvm,
                        char *package_name, char *name);
void zvm_dynamic_load(ZVM_VirtualMachine *zvm,
                      ZVM_Executable *caller_exe, Function *caller, int pc,
                      Function *func);
/* util.c */
void zvm_vstr_clear(VString *v);
void zvm_vstr_append_string(VString *v, ZVM_Char *str);
void zvm_vstr_append_character(VString *v, ZVM_Char ch);
void zvm_initialize_value(ZVM_TypeSpecifier *type, ZVM_Value *value);

/* error.c */
void zvm_error_i(ZVM_Executable *exe, Function *func,
                 int pc, RuntimeError id, ...);
void zvm_error_n(ZVM_VirtualMachine *zvm, RuntimeError id, ...);
int zvm_conv_pc_to_line_number(ZVM_Executable *exe, Function *func, int pc);
void zvm_format_message(ZVM_ErrorDefinition *error_definition,
                        int id, VString *message, va_list ap);

extern OpcodeInfo zvm_opcode_info[];
extern ZVM_ObjectRef zvm_null_object_ref;
extern ZVM_ErrorDefinition zvm_error_message_format[];

#endif /* ZVM_PRI_H_INCLUDED */
