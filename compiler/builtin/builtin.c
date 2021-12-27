#include <stdio.h>
#include "../zeroc.h"


char *st_zero_lang_zeh_text[] = {
    "void print(string str);",
    "void println(string str) {",
    "    print(str + \"\\n\");",
    "}",
    "",
    "public class File {",
    "    native_pointer fp;",
    "    constructor initialize(native_pointer fp) {",
    "\tthis.fp = fp;",
    "    }",
    "}",
    "",
    "File fopen(string file_name, string mode);",
    "string fgets(File file);",
    "void fputs(string str, File file);",
    "void fclose(File file);",
    "",
    "double to_double(int int_value) {",
    "    return int_value;",
    "}",
    "",
    "int to_int(double double_value) {",
    "    return double_value;",
    "}",
    "",
    "class StackTrace {",
    "    int\tline_number;",
    "    string file_name;",
    "    string function_name;",
    "}",
    "",
    "abstract public class Exception {",
    "    public string message;",
    "    public StackTrace[] stack_trace;",
    "    public void print_stack_trace() {",
    "\tint i;",
    "\tprintln(\"Exception occured. \" + this.message);",
    "\tfor (i = 0; i < this.stack_trace.size(); i++) {",
    "\t    println(\"\\tat \" ",
    "\t\t    + this.stack_trace[i].function_name",
    "\t\t    + \"(\" + this.stack_trace[i].file_name",
    "\t\t    + \":\" + this.stack_trace[i].line_number + \")\");",
    "\t}",
    "    }",
    "    public virtual constructor initialize() {",
    "\tthis.stack_trace = new StackTrace[0];",
    "    }",
    "}",
    "",
    "abstract public class BugException : Exception {",
    "}",
    "",
    "abstract public class RuntimeException : Exception {",
    "}",
    "",
    "public class NullPointerException : BugException {",
    "}",
    "",
    "public class ArrayIndexOutOfBoundsException : BugException {",
    "}",
    "",
    "public class StringIndexOutOfBoundsException : BugException {",
    "}",
    "",
    "public class DivisionByZeroException : RuntimeException {",
    "}",
    "",
    "public class MultibyteCharacterConvertionException : RuntimeException {",
    "}",
    "",
    "public class ClassCastException : BugException {",
    "}",
    NULL
};
char *st_zero_lang_ze_text[] = {
    "require zero.lang;",
    "",
    "native_pointer __fopen(string file_name, string mode);",
    "string __fgets(native_pointer fp);",
    "void __fputs(string str, native_pointer fp);",
    "void __fclose(native_pointer fp);",
    "",
    "File fopen(string file_name, string mode) {",
    "    native_pointer fp = __fopen(file_name, mode);",
    "    if (fp == null) {",
    "\treturn null;",
    "    } else {",
    "\treturn new File(fp);",
    "    }",
    "}",
    "",
    "string fgets(File file) {",
    "    return __fgets(file.fp);",
    "}",
    "",
    "void fputs(string str, File file) {",
    "    __fputs(str, file.fp);",
    "}",
    "",
    "void fclose(File file) {",
    "    __fclose(file.fp);",
    "}",
    NULL
};

BuiltinScript zec_builtin_script[] = {
    {"zero.lang", ZEH_SOURCE, st_zero_lang_zeh_text},
    {"zero.lang", ZEM_SOURCE, st_zero_lang_ze_text},
    {NULL, ZEM_SOURCE, NULL}
};
