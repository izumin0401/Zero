#ifndef PUBLIC_ZVM_H_INCLUDED
#define PUBLIC_ZVM_H_INCLUDED

typedef struct ZVM_Executable_tag ZVM_Executable;
typedef struct ZVM_ExecutableList_tag ZVM_ExecutableList;
typedef struct ZVM_VirtualMachine_tag ZVM_VirtualMachine;

typedef struct ZVM_Object_tag ZVM_Object;
typedef struct ZVM_String_tag ZVM_String;

typedef enum {
    ZVM_FALSE = 0,
    ZVM_TRUE = 1
} ZVM_Boolean;

typedef struct ZVM_VTable_tag ZVM_VTable;

typedef struct {
    ZVM_VTable *v_table;
    ZVM_Object *data;
} ZVM_ObjectRef;

typedef union {
    int                 int_value;
    double              double_value;
    ZVM_ObjectRef       object;
} ZVM_Value;

ZVM_VirtualMachine *ZVM_create_virtual_machine(void);
void ZVM_set_executable(ZVM_VirtualMachine *zvm, ZVM_ExecutableList *list);
ZVM_Value ZVM_execute(ZVM_VirtualMachine *zvm);
void ZVM_dispose_executable_list(ZVM_ExecutableList *list);
void ZVM_dispose_virtual_machine(ZVM_VirtualMachine *zvm);

#endif /* PUBLIC_ZVM_CODE_H_INCLUDED */
