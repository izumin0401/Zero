#ifndef PUBLIC_ZVM_CODE_H_INCLUDED
#define PUBLIC_ZVM_CODE_H_INCLUDED
#include <stdio.h>
#include <wchar.h>
#include "ZVM.h"

typedef enum {
    ZVM_VOID_TYPE,
    ZVM_BOOLEAN_TYPE,
    ZVM_INT_TYPE,
    ZVM_DOUBLE_TYPE,
    ZVM_STRING_TYPE,
    ZVM_CLASS_TYPE,
    ZVM_DELEGATE_TYPE,
    ZVM_ENUM_TYPE,
    ZVM_NULL_TYPE,
    ZVM_NATIVE_POINTER_TYPE,
    ZVM_BASE_TYPE,
    ZVM_UNSPECIFIED_IDENTIFIER_TYPE
} ZVM_BasicType;

typedef struct ZVM_TypeSpecifier_tag ZVM_TypeSpecifier;

typedef struct {
    char                *name;
    ZVM_TypeSpecifier   *type;
} ZVM_LocalVariable;

typedef enum {
    ZVM_FUNCTION_DERIVE,
    ZVM_ARRAY_DERIVE
} ZVM_DeriveTag;

typedef struct {
    int                 parameter_count;
    ZVM_LocalVariable   *parameter;
} ZVM_FunctionDerive;

typedef struct {
    int dummy; /* make compiler happy */
} ZVM_ArrayDerive;

typedef struct ZVM_TypeDerive_tag {
    ZVM_DeriveTag       tag;
    union {
        ZVM_FunctionDerive      function_d;
    } u;
} ZVM_TypeDerive;

struct ZVM_TypeSpecifier_tag {
    ZVM_BasicType       basic_type;
    union {
        struct {
            int index;
        } class_t;
        struct {
            int dummy;
        } delegate_t;
        struct {
            int index;
        } enum_t;
    } u;
    int                 derive_count;
    ZVM_TypeDerive      *derive;
};

typedef wchar_t ZVM_Char;
typedef unsigned char ZVM_Byte;

typedef enum {
    ZVM_PUSH_INT_1BYTE = 1,
    ZVM_PUSH_INT_2BYTE,
    ZVM_PUSH_INT,
    ZVM_PUSH_DOUBLE_0,
    ZVM_PUSH_DOUBLE_1,
    ZVM_PUSH_DOUBLE,
    ZVM_PUSH_STRING,
    ZVM_PUSH_NULL,
    /**********/
    ZVM_PUSH_STACK_INT,
    ZVM_PUSH_STACK_DOUBLE,
    ZVM_PUSH_STACK_OBJECT,
    ZVM_POP_STACK_INT,
    ZVM_POP_STACK_DOUBLE,
    ZVM_POP_STACK_OBJECT,
    /**********/
    ZVM_PUSH_STATIC_INT,
    ZVM_PUSH_STATIC_DOUBLE,
    ZVM_PUSH_STATIC_OBJECT,
    ZVM_POP_STATIC_INT,
    ZVM_POP_STATIC_DOUBLE,
    ZVM_POP_STATIC_OBJECT,
    /**********/
    ZVM_PUSH_ARRAY_INT,
    ZVM_PUSH_ARRAY_DOUBLE,
    ZVM_PUSH_ARRAY_OBJECT,
    ZVM_POP_ARRAY_INT,
    ZVM_POP_ARRAY_DOUBLE,
    ZVM_POP_ARRAY_OBJECT,
    /**********/
    ZVM_PUSH_FIELD_INT,
    ZVM_PUSH_FIELD_DOUBLE,
    ZVM_PUSH_FIELD_OBJECT,
    ZVM_POP_FIELD_INT,
    ZVM_POP_FIELD_DOUBLE,
    ZVM_POP_FIELD_OBJECT,
    /**********/
    ZVM_ADD_INT,
    ZVM_ADD_DOUBLE,
    ZVM_ADD_STRING,
    ZVM_SUB_INT,
    ZVM_SUB_DOUBLE,
    ZVM_MUL_INT,
    ZVM_MUL_DOUBLE,
    ZVM_DIV_INT,
    ZVM_DIV_DOUBLE,
    ZVM_MOD_INT,
    ZVM_MOD_DOUBLE,
    ZVM_MINUS_INT,
    ZVM_MINUS_DOUBLE,
    ZVM_INCREMENT,
    ZVM_DECREMENT,
    ZVM_CAST_INT_TO_DOUBLE,
    ZVM_CAST_DOUBLE_TO_INT,
    ZVM_CAST_BOOLEAN_TO_STRING,
    ZVM_CAST_INT_TO_STRING,
    ZVM_CAST_DOUBLE_TO_STRING,
    ZVM_CAST_ENUM_TO_STRING,
    ZVM_UP_CAST,
    ZVM_DOWN_CAST,
    ZVM_EQ_INT,
    ZVM_EQ_DOUBLE,
    ZVM_EQ_OBJECT,
    ZVM_EQ_STRING,
    ZVM_GT_INT,
    ZVM_GT_DOUBLE,
    ZVM_GT_STRING,
    ZVM_GE_INT,
    ZVM_GE_DOUBLE,
    ZVM_GE_STRING,
    ZVM_LT_INT,
    ZVM_LT_DOUBLE,
    ZVM_LT_STRING,
    ZVM_LE_INT,
    ZVM_LE_DOUBLE,
    ZVM_LE_STRING,
    ZVM_NE_INT,
    ZVM_NE_DOUBLE,
    ZVM_NE_OBJECT,
    ZVM_NE_STRING,
    ZVM_LOGICAL_AND,
    ZVM_LOGICAL_OR,
    ZVM_LOGICAL_NOT,
    ZVM_POP,
    ZVM_DUPLICATE,
    ZVM_DUPLICATE_OFFSET,
    ZVM_JUMP,
    ZVM_JUMP_IF_TRUE,
    ZVM_JUMP_IF_FALSE,
    /**********/
    ZVM_PUSH_FUNCTION,
    ZVM_PUSH_METHOD,
    ZVM_PUSH_DELEGATE,
    ZVM_INVOKE,
    ZVM_INVOKE_DELEGATE,
    ZVM_RETURN,
    /**********/
    ZVM_NEW,
    ZVM_NEW_ARRAY,
    ZVM_NEW_ARRAY_LITERAL_INT,
    ZVM_NEW_ARRAY_LITERAL_DOUBLE,
    ZVM_NEW_ARRAY_LITERAL_OBJECT,
    ZVM_SUPER,
    ZVM_INSTANCEOF,
    ZVM_THROW,
    ZVM_RETHROW,
    ZVM_GO_FINALLY,
    ZVM_FINALLY_END
} ZVM_Opcode;

typedef enum {
    ZVM_CONSTANT_INT,
    ZVM_CONSTANT_DOUBLE,
    ZVM_CONSTANT_STRING
} ZVM_ConstantPoolTag;

typedef struct {
    ZVM_ConstantPoolTag tag;
    union {
        int     c_int;
        double  c_double;
        ZVM_Char *c_string;
    } u;
} ZVM_ConstantPool;

typedef struct {
    char                *name;
    ZVM_TypeSpecifier   *type;
} ZVM_Variable;

typedef struct {
    int line_number;
    int start_pc;
    int pc_count;
} ZVM_LineNumber;

typedef struct {
    int class_index;
    int start_pc;
    int end_pc;
} ZVM_CatchClause;

typedef struct {
    int                 try_start_pc;
    int                 try_end_pc;
    int                 catch_count;
    ZVM_CatchClause     *catch_clause;
    int                 finally_start_pc;
    int                 finally_end_pc;
} ZVM_Try;

typedef struct {
    int                 code_size;
    ZVM_Byte            *code;
    int                 line_number_size;
    ZVM_LineNumber      *line_number;
    int                 try_size;
    ZVM_Try             *try;
    int                 need_stack_size;
} ZVM_CodeBlock;

typedef struct {
    ZVM_TypeSpecifier   *type;
    char                *package_name;
    char                *name;
    int                 parameter_count;
    ZVM_LocalVariable   *parameter;
    ZVM_Boolean         is_implemented;
    ZVM_Boolean         is_method;
    int                 local_variable_count;
    ZVM_LocalVariable   *local_variable;
    ZVM_CodeBlock       code_block;
} ZVM_Function;

typedef enum {
    ZVM_FILE_ACCESS,
    ZVM_PUBLIC_ACCESS,
    ZVM_PRIVATE_ACCESS
} ZVM_AccessModifier;

typedef struct {
    ZVM_AccessModifier  access_modifier;
    char                *name;
    ZVM_TypeSpecifier   *type;
} ZVM_Field;

typedef struct {
    ZVM_AccessModifier  access_modifier;
    ZVM_Boolean         is_abstract;
    ZVM_Boolean         is_virtual;
    ZVM_Boolean         is_override;
    char                *name;
} ZVM_Method;

typedef enum {
    ZVM_CLASS_DEFINITION,
    ZVM_INTERFACE_DEFINITION
} ZVM_ClassOrInterface;

typedef struct {
    char *package_name;
    char *name;
} ZVM_ClassIdentifier;

typedef struct {
    ZVM_Boolean                 is_abstract;
    ZVM_AccessModifier          access_modifier;
    ZVM_ClassOrInterface        class_or_interface;
    char                        *package_name;
    char                        *name;
    ZVM_Boolean                 is_implemented;
    ZVM_ClassIdentifier         *super_class;
    int                         interface_count;
    ZVM_ClassIdentifier         *interface;
    int                         field_count;
    ZVM_Field                   *field;
    int                         method_count;
    ZVM_Method                  *method;
} ZVM_Class;

typedef struct {
    char        *name;
    int         enumerator_count;
    char        **enumerator;
} ZVM_Enum;

struct ZVM_Executable_tag {
    char                *package_name;
    ZVM_Boolean         is_required;
    char                *path;
    int                 constant_pool_count;
    ZVM_ConstantPool    *constant_pool;
    int                 global_variable_count;
    ZVM_Variable        *global_variable;
    int                 function_count;
    ZVM_Function        *function;
    int                 type_specifier_count;
    ZVM_TypeSpecifier   *type_specifier;
    int                 class_count;
    ZVM_Class           *class_definition;
    int                 enum_count;
    ZVM_Enum            *enum_definition;
    ZVM_CodeBlock       top_level;
};

typedef struct ZVM_ExecutableItem_tag {
    ZVM_Executable *executable;
    struct ZVM_ExecutableItem_tag *next;
} ZVM_ExecutableItem;

struct ZVM_ExecutableList_tag {
    ZVM_Executable      *top_level;
    ZVM_ExecutableItem  *list;
};

#endif /* PUBLIC_ZVM_CODE_H_INCLUDED */
