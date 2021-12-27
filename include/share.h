#ifndef PRIVATE_SHARE_H_INCLUDED
#define PRIVATE_SHARE_H_INCLUDED
#include <stdio.h>
#include "ZVM_code.h"
#include "ZEC.h"

typedef struct {
    char        *mnemonic;
    char        *parameter;
    int         stack_increment;
} OpcodeInfo;

typedef enum {
    SEARCH_FILE_SUCCESS,
    SEARCH_FILE_NOT_FOUND,
    SEARCH_FILE_PATH_TOO_LONG
} SearchFileStatus;

#ifdef ZEO_WINDOWS
#define FILE_SEPARATOR          ('\\')
#define FILE_PATH_SEPARATOR     (';')
#else
#define FILE_SEPARATOR          ('/')
#define FILE_PATH_SEPARATOR     (':')
#endif

#define ZERO_REQUIRE_SUFFIX   (".zeh")
#define ZERO_IMPLEMENTATION_SUFFIX    (".ze")
#define ZERO_DEFAULT_PACKAGE_P1  "zero"
#define ZERO_DEFAULT_PACKAGE_P2  "lang"
#define ZERO_DEFAULT_PACKAGE \
 (ZERO_DEFAULT_PACKAGE_P1 "." ZERO_DEFAULT_PACKAGE_P2)
#define ZERO_STACK_TRACE_CLASS ("StackTrace")
#define ZERO_PRINT_STACK_TRACE_FUNC ("Exception#print_stack_trace")
#define EXCEPTION_CLASS_NAME ("Exception")
#define ARRAY_INDEX_EXCEPTION_NAME ("ArrayIndexOutOfBoundsException")
#define STRING_INDEX_EXCEPTION_NAME ("StringIndexOutOfBoundsException")
#define NULL_POINTER_EXCEPTION_NAME ("NullPointerException")
#define DIVISION_BY_ZERO_EXCEPTION_NAME ("DivisionByZeroException")
#define CLASS_CAST_EXCEPTION_NAME ("ClassCastException")
#define MULTIBYTE_CONVERTION_EXCEPTION_NAME \
  ("MultibyteCharacterConvertionException")

#define ARRAY_METHOD_SIZE "size"
#define ARRAY_METHOD_RESIZE "resize"
#define ARRAY_METHOD_INSERT "insert"
#define ARRAY_METHOD_REMOVE "remove"
#define ARRAY_METHOD_ADD "add"
#define ARRAY_PREFIX "array#"

#define STRING_METHOD_LENGTH "length"
#define STRING_METHOD_SUBSTR "substr"
#define STRING_PREFIX "string#"

#define ARRAY_SIZE(array)  (sizeof(array) / sizeof((array)[0]))

/* dispose.c */
void zvm_dispose_executable(ZVM_Executable *exe);
/* wchar.c */
size_t zvm_wcslen(wchar_t *str);
wchar_t *zvm_wcscpy(wchar_t *dest, wchar_t *src);
wchar_t *zvm_wcsncpy(wchar_t *dest, wchar_t *src, size_t n);
int zvm_wcscmp(wchar_t *s1, wchar_t *s2);
wchar_t *zvm_wcscat(wchar_t *s1, wchar_t *s2);
int zvm_mbstowcs_len(const char *src);
void zvm_mbstowcs(const char *src, wchar_t *dest);
wchar_t *zvm_mbstowcs_alloc(ZVM_VirtualMachine *zvm, const char *src);
int zvm_wcstombs_len(const wchar_t *src);
void zvm_wcstombs_i(const wchar_t *src, char *dest);
char *zvm_wcstombs_alloc(const wchar_t *src);
char zvm_wctochar(wchar_t src);
int zvm_print_wcs(FILE *fp, wchar_t *str);
int zvm_print_wcs_ln(FILE *fp, wchar_t *str);
ZVM_Boolean zvm_iswdigit(wchar_t ch);
/* disassemble.c */
int zvm_dump_instruction(FILE *fp, ZVM_Byte *code, int index);
void zvm_disassemble(ZVM_Executable *exe);
/* util.c */
SearchFileStatus
zvm_search_file(char *search_path, char *search_file,
                char *found_path, FILE **fp);
ZVM_Boolean zvm_compare_string(char *str1, char *str2);
ZVM_Boolean zvm_compare_package_name(char *p1, char *p2);
char *
zvm_create_method_function_name(char *class_name, char *method_name);
void zvm_strncpy(char *dest, char *src, int buf_size);

SearchFileStatus zec_dynamic_compile(ZEC_Compiler *compiler,
                                     char *package_name,
                                     ZVM_ExecutableList *list,
                                     ZVM_ExecutableItem **add_top,
                                     char *search_file);

#endif  /* PRIVATE_SHARE_H_INCLUDED */
