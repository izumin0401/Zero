#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zeroc.h"

static CompilerList *st_compiler_list = NULL;
extern BuiltinScript zec_builtin_script[];

typedef struct {
    char *name;
    ZVM_BasicType type;
} BuiltInMethodParameter;

typedef struct {
    char *name;
    ZVM_BasicType return_type;
    BuiltInMethodParameter *parameter;
    int parameter_count;
} BuiltInMethod;

static BuiltInMethodParameter st_array_size_arg[] = {};
static BuiltInMethodParameter st_array_resize_arg[] = {
    {"new_size", ZVM_INT_TYPE}
};
static BuiltInMethodParameter st_array_insert_arg[] = {
    {"index", ZVM_INT_TYPE},
    {"new_item", ZVM_BASE_TYPE}
};
static BuiltInMethodParameter st_array_remove_arg[] = {
    {"index", ZVM_INT_TYPE}
};
static BuiltInMethodParameter st_array_add_arg[] = {
    {"new_item", ZVM_BASE_TYPE}
};

static BuiltInMethod st_array_method[] = {
    {ARRAY_METHOD_SIZE, ZVM_INT_TYPE, st_array_size_arg,
     ARRAY_SIZE(st_array_size_arg)},
    {ARRAY_METHOD_RESIZE, ZVM_VOID_TYPE, st_array_resize_arg,
     ARRAY_SIZE(st_array_resize_arg)},
    {ARRAY_METHOD_INSERT, ZVM_VOID_TYPE, st_array_insert_arg,
     ARRAY_SIZE(st_array_insert_arg)},
    {ARRAY_METHOD_REMOVE, ZVM_VOID_TYPE, st_array_remove_arg,
     ARRAY_SIZE(st_array_remove_arg)},
    {ARRAY_METHOD_ADD, ZVM_VOID_TYPE, st_array_add_arg,
     ARRAY_SIZE(st_array_add_arg)},
};

static BuiltInMethodParameter st_string_length_arg[] = {};
static BuiltInMethodParameter st_string_substr_arg[] = {
    {"start", ZVM_INT_TYPE},
    {"length", ZVM_INT_TYPE}
};

static BuiltInMethod st_string_method[] = {
    {"length", ZVM_INT_TYPE, st_string_length_arg,
     ARRAY_SIZE(st_string_length_arg)},
    {"substr", ZVM_STRING_TYPE, st_string_substr_arg,
     ARRAY_SIZE(st_string_substr_arg)},
};

static FunctionDefinition *
create_built_in_method(BuiltInMethod *src, int method_count)
{
    int i;
    int param_idx;
    ParameterList *param_list;
    FunctionDefinition *fd_array;

    fd_array = zec_malloc(sizeof(FunctionDefinition) * method_count);
    for (i = 0; i < method_count; i++) {
        fd_array[i].name = src[i].name;
        fd_array[i].type = zec_alloc_type_specifier(src[i].return_type);
        param_list = NULL;
        for (param_idx = 0; param_idx < src[i].parameter_count; param_idx++) {
            TypeSpecifier *type
                = zec_alloc_type_specifier(src[i].parameter[param_idx].type);
            if (param_list) {
                param_list
                    = zec_chain_parameter(param_list, type,
                                          src[i].parameter[param_idx].name);
            } else {
                param_list
                    = zec_create_parameter(type,
                                           src[i].parameter[param_idx].name);
            }
        }
        fd_array[i].parameter = param_list;
        fd_array[i].throws = NULL;
    }
    return fd_array;
}

ZEC_Compiler *
ZEC_create_compiler(void)
{
    MEM_Storage storage;
    ZEC_Compiler *compiler;
    ZEC_Compiler *compiler_backup;

    compiler_backup = zec_get_current_compiler();

    storage = MEM_open_storage(0);
    compiler = MEM_storage_malloc(storage,
                                  sizeof(struct ZEC_Compiler_tag));
    zec_set_current_compiler(compiler);
    compiler->compile_storage = storage;
    compiler->package_name = NULL;
    compiler->source_suffix = ZEM_SOURCE;
    compiler->require_list = NULL;
    compiler->rename_list = NULL;
    compiler->function_list = NULL;
    compiler->zvm_function_count = 0;
    compiler->zvm_function = NULL;
    compiler->zvm_class_count = 0;
    compiler->zvm_class = NULL;
    compiler->declaration_list = NULL;
    compiler->statement_list = NULL;
    compiler->class_definition_list = NULL;
    compiler->delegate_definition_list = NULL;
    compiler->enum_definition_list = NULL;
    compiler->current_block = NULL;
    compiler->current_line_number = 1;
    compiler->current_class_definition = NULL;
    compiler->current_catch_clause = NULL;
    compiler->input_mode = FILE_INPUT_MODE;
    compiler->required_list = NULL;
    compiler->array_method_count = ARRAY_SIZE(st_array_method);
    compiler->array_method
        = create_built_in_method(st_array_method,
                                 ARRAY_SIZE(st_array_method));
    compiler->string_method_count = ARRAY_SIZE(st_string_method);
    compiler->string_method
        = create_built_in_method(st_string_method,
                                 ARRAY_SIZE(st_string_method));

    zec_set_current_compiler(compiler_backup);

    return compiler;
}

static CompilerList *
add_compiler_to_list(CompilerList *list, ZEC_Compiler *compiler)
{
    CompilerList *pos;
    CompilerList *new_item;

    new_item = MEM_malloc(sizeof(CompilerList));
    new_item->compiler = compiler;
    new_item->next = NULL;

    if (list == NULL) {
        return new_item;
    }
    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = new_item;

    return list;
}

static ZEC_Compiler *
search_compiler(CompilerList *list, PackageName *package_name)
{
    CompilerList *pos;

    for (pos = list; pos; pos = pos->next) {
        if (zec_compare_package_name(pos->compiler->package_name,
                                     package_name))
            break;
    }

    if (pos) {
        return pos->compiler;
    } else {
        return NULL;
    }
}

static ZVM_Boolean
search_buitin_source(char *package_name, SourceSuffix suffix,
                     SourceInput *input)
{
    int i;

    for (i = 0; zec_builtin_script[i].source_string != NULL; i++) {
        if (zvm_compare_string(package_name,
                               zec_builtin_script[i].package_name)
            && zec_builtin_script[i].suffix == suffix) {
            input->input_mode = STRING_INPUT_MODE;
            input->u.string.lines = zec_builtin_script[i].source_string;
            return ZVM_TRUE;
        }
    }
    return ZVM_FALSE;
}

static void
make_search_path(int line_number, PackageName *package_name, char *buf)
{
    PackageName *pos;
    int len = 0;
    int prev_len = 0;
    int suffix_len;

    suffix_len = strlen(ZERO_REQUIRE_SUFFIX);
    buf[0] = '\0';
    for (pos = package_name; pos; pos = pos->next) {
        prev_len = len;
        len += strlen(pos->name);
        if (len > FILENAME_MAX - (2 + suffix_len)) {
            zec_compile_error(line_number,
                              PACKAGE_NAME_TOO_LONG_ERR,
                              MESSAGE_ARGUMENT_END);
        }
        strcpy(&buf[prev_len], pos->name);
        if (pos->next) {
            buf[strlen(buf)] = FILE_SEPARATOR;
            len++;
        }
    }
    strcpy(&buf[len], ZERO_REQUIRE_SUFFIX);
}

static void
make_search_path_impl(char *package_name, char *buf)
{
    int suffix_len;
    int package_len;
    int i;

    suffix_len = strlen(ZERO_IMPLEMENTATION_SUFFIX);
    package_len = strlen(package_name);

    DBG_assert(package_len <= FILENAME_MAX - (2 + suffix_len),
               ("package name is too long(%s)", package_name));

    for (i = 0; package_name[i] != '\0'; i++) {
        if (package_name[i] == '.') {
            buf[i] = FILE_SEPARATOR;
        } else {
            buf[i] = package_name[i];
        }
    }
    buf[i] = '\0';
    strcat(buf, ZERO_IMPLEMENTATION_SUFFIX);
}

static void
get_require_input(RequireList *req, char *found_path,
                  SourceInput *source_input)
{
    char *search_path;
    char search_file[FILENAME_MAX];
    FILE *fp;
    char *package_name;
    SearchFileStatus status;

    package_name = zec_package_name_to_string(req->package_name);
    if (search_buitin_source(package_name, ZEH_SOURCE, source_input)) {
        MEM_free(package_name);
        found_path[0] = '\0';
        return;
    }
    MEM_free(package_name);

    search_path = getenv("ZEM_REQUIRE_SEARCH_PATH");
    if (search_path == NULL) {
        search_path = ".";
    }
    make_search_path(req->line_number, req->package_name, search_file);

    status = zvm_search_file(search_path, search_file, found_path, &fp);
    
    if (status != SEARCH_FILE_SUCCESS) {
        if (status == SEARCH_FILE_NOT_FOUND) {
            zec_compile_error(req->line_number,
                              REQUIRE_FILE_NOT_FOUND_ERR,
                              STRING_MESSAGE_ARGUMENT, "file", search_file,
                              MESSAGE_ARGUMENT_END);
        } else {
            zec_compile_error(req->line_number,
                              REQUIRE_FILE_ERR,
                              INT_MESSAGE_ARGUMENT, "status", (int)status,
                              MESSAGE_ARGUMENT_END);
        }
    }
    source_input->input_mode = FILE_INPUT_MODE;
    source_input->u.file.fp = fp;
}

static ZVM_Boolean
add_exe_to_list(ZVM_Executable *exe, ZVM_ExecutableList *list)
{
    ZVM_ExecutableItem *new_item;
    ZVM_ExecutableItem *pos;
    ZVM_ExecutableItem *tail;

    for (pos = list->list; pos; pos = pos->next) {
        if (zvm_compare_package_name(pos->executable->package_name,
                                     exe->package_name)
            && pos->executable->is_required == exe->is_required) {
            return ZVM_FALSE;
        }
        tail = pos;
    }

    new_item = MEM_malloc(sizeof(ZVM_ExecutableItem));
    new_item->executable = exe;
    new_item->next = NULL;

    if (list->list == NULL) {
        list->list = new_item;
    } else {
        tail->next = new_item;
    }

    return ZVM_TRUE;
}

static void
set_path_to_compiler(ZEC_Compiler *compiler, char *path)
{
    compiler->path = MEM_storage_malloc(compiler->compile_storage,
                                        strlen(path) + 1);
    strcpy(compiler->path, path);
}

static ZVM_Executable *
do_compile(ZEC_Compiler *compiler, ZVM_ExecutableList *list,
           char *path, ZVM_Boolean is_required)
{
    extern FILE *yyin;
    extern int yyparse(void);
    RequireList *req_pos;
    ZEC_Compiler *req_comp;
    ZVM_Executable *exe;
    ZVM_Executable *req_exe;
    char found_path[FILENAME_MAX];
    ZEC_Compiler *compiler_backup;
    SourceInput source_input;

    compiler_backup = zec_get_current_compiler();
    zec_set_current_compiler(compiler);
    if (yyparse()) {
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }

    for (req_pos = compiler->require_list; req_pos;
         req_pos = req_pos->next) {
        req_comp = search_compiler(st_compiler_list, req_pos->package_name);
        if (req_comp) {
            compiler->required_list
                = add_compiler_to_list(compiler->required_list, req_comp);
            continue;
        }
        req_comp = ZEC_create_compiler();
        
        /* BUGBUG req_comp references parent compiler's MEM_storage */
        req_comp->package_name = req_pos->package_name;
        req_comp->source_suffix = ZEH_SOURCE;

        compiler->required_list
            = add_compiler_to_list(compiler->required_list, req_comp);
        st_compiler_list = add_compiler_to_list(st_compiler_list, req_comp);

        get_require_input(req_pos, found_path, &source_input);
        set_path_to_compiler(req_comp, found_path);
        req_comp->input_mode = source_input.input_mode;
        if (source_input.input_mode == FILE_INPUT_MODE) {
            yyin = source_input.u.file.fp;
        } else {
            zec_set_source_string(source_input.u.string.lines);
        }
        req_exe = do_compile(req_comp, list, found_path, ZVM_TRUE);
    }

    zec_fix_tree(compiler);
    exe = zec_generate(compiler);
    if (path) {
        exe->path = MEM_strdup(path);
    } else {
        exe->path = NULL;
    }
    /*zvm_disassemble(exe);*/

    exe->is_required = is_required;
    if (!add_exe_to_list(exe, list)) {
        zvm_dispose_executable(exe);
    }

    zec_set_current_compiler(compiler_backup);

    return exe;
}

static void
dispose_compiler_list(void)
{
    CompilerList *temp;

    while (st_compiler_list) {
        temp = st_compiler_list;
        st_compiler_list = temp->next;
        MEM_free(temp);
    }
}


ZVM_ExecutableList *
ZEC_compile(ZEC_Compiler *compiler, FILE *fp, char *path)
{
    extern FILE *yyin;
    ZVM_ExecutableList *list;
    ZVM_Executable *exe;

    DBG_assert(st_compiler_list == NULL,
               ("st_compiler_list != NULL(%p)", st_compiler_list));
    set_path_to_compiler(compiler, path);
    compiler->input_mode = FILE_INPUT_MODE;

    yyin = fp;

    list = MEM_malloc(sizeof(ZVM_ExecutableList));
    list->list = NULL;
    
    exe = do_compile(compiler, list, NULL, ZVM_FALSE);
    exe->path = MEM_strdup(path);
    list->top_level = exe;

    /* zvm_disassemble(exe); */

    dispose_compiler_list();
    zec_reset_string_literal_buffer();

    return list;
}

PackageName *
create_one_package_name(ZEC_Compiler *compiler,
                        char *str, int start_idx, int to_idx)
{
    PackageName *pn;
    int i;

    MEM_Storage storage = compiler->compile_storage;
    pn = MEM_storage_malloc(storage, sizeof(PackageName));
    pn->name = MEM_storage_malloc(storage, to_idx - start_idx + 1);

    for (i = 0; i < to_idx - start_idx; i++) {
        pn->name[i] = str[start_idx + i];
    }
    pn->name[i] = '\0';
    pn->next = NULL;

    return pn;
}

static PackageName *
string_to_package_name(ZEC_Compiler *compiler, char *str)
{
    int start_idx;
    int i;
    PackageName *pn;
    PackageName *top;
    PackageName *tail = NULL;

    for (start_idx = i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            pn = create_one_package_name(compiler, str, start_idx, i);
            start_idx = i + 1;
            if (tail) {
                tail->next = pn;
            } else {
                top = pn;
            }
            tail = pn;
        }
    }
    pn = create_one_package_name(compiler, str, start_idx, i);
    if (tail) {
        tail->next = pn;
    } else {
        top = pn;
    }

    return top;
}

SearchFileStatus
get_dynamic_load_input(char *package_name, char *found_path,
                       char *search_file, SourceInput *source_input)
{
    SearchFileStatus status;
    char *search_path;
    FILE *fp;

    if (search_buitin_source(package_name, ZEM_SOURCE, source_input)) {
        found_path[0] = '\0';
        return SEARCH_FILE_SUCCESS;
    }

    search_path = getenv("ZEM_LOAD_SEARCH_PATH");
    if (search_path == NULL) {
        search_path = ".";
    }
    make_search_path_impl(package_name, search_file);
    status = zvm_search_file(search_path, search_file, found_path, &fp);

    if (status != SEARCH_FILE_SUCCESS) {
        return status;
    }
    source_input->input_mode = FILE_INPUT_MODE;
    source_input->u.file.fp = fp;

    return SEARCH_FILE_SUCCESS;
}

SearchFileStatus
zec_dynamic_compile(ZEC_Compiler *compiler, char *package_name,
                    ZVM_ExecutableList *list, ZVM_ExecutableItem **add_top,
                    char *search_file)
{
    SearchFileStatus status;
    extern FILE *yyin;
    ZVM_ExecutableItem *tail;
    ZVM_Executable *exe;
    SourceInput source_input;
    char found_path[FILENAME_MAX];
        
    status = get_dynamic_load_input(package_name, found_path,
                                    search_file, &source_input);
    if (status != SEARCH_FILE_SUCCESS) {
        return status;
    }
    DBG_assert(st_compiler_list == NULL,
               ("st_compiler_list != NULL(%p)", st_compiler_list));

    for (tail = list->list; tail->next; tail = tail->next)
        ;

    compiler->package_name = string_to_package_name(compiler, package_name);
    set_path_to_compiler(compiler, found_path);

    compiler->input_mode = source_input.input_mode;
    if (source_input.input_mode == FILE_INPUT_MODE) {
        yyin = source_input.u.file.fp;
    } else {
        zec_set_source_string(source_input.u.string.lines);
    }
    exe = do_compile(compiler, list, found_path, ZVM_FALSE);

    dispose_compiler_list();
    zec_reset_string_literal_buffer();

    *add_top = tail->next;

    return SEARCH_FILE_SUCCESS;
}

static CompilerList *
traversal_compiler(CompilerList *list, ZEC_Compiler *compiler)
{
    CompilerList *list_pos;
    CompilerList *req_pos;

    for (list_pos = list; list_pos; list_pos = list_pos->next) {
        if (list_pos->compiler == compiler)
            break;
    }
    if (list_pos == NULL) {
        list = add_compiler_to_list(list, compiler);
    }
    for (req_pos = compiler->required_list; req_pos; req_pos = req_pos->next) {
        list = traversal_compiler(list, req_pos->compiler);
    }

    return list;
}

void
ZEC_dispose_compiler(ZEC_Compiler *compiler)
{
    CompilerList *list = NULL;
    CompilerList *pos;
    FunctionDefinition *fd_pos;
    CompilerList *temp;

    list = traversal_compiler(list, compiler);

    for (pos = list; pos; ) {
        for (fd_pos = pos->compiler->function_list; fd_pos;
             fd_pos = fd_pos->next) {
            MEM_free(fd_pos->local_variable);
        }
        while (pos->compiler->required_list) {
            temp = pos->compiler->required_list;
            pos->compiler->required_list = temp->next;
            MEM_free(temp);
        }
        MEM_dispose_storage(pos->compiler->compile_storage);
        temp = pos->next;
        MEM_free(pos);
        pos = temp;
    }
}
