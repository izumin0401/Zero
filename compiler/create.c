#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zeroc.h"

DeclarationList *
zec_chain_declaration(DeclarationList *list, Declaration *decl)
{
    DeclarationList *new_item;
    DeclarationList *pos;

    new_item = zec_malloc(sizeof(DeclarationList));
    new_item->declaration = decl;
    new_item->next = NULL;

    if (list == NULL) {
        return new_item;
    }

    for (pos = list; pos->next != NULL; pos = pos->next)
        ;
    pos->next = new_item;

    return list;
}

Declaration *
zec_alloc_declaration(ZVM_Boolean is_final, TypeSpecifier *type,
                      char *identifier)
{
    Declaration *decl;

    decl = zec_malloc(sizeof(Declaration));
    decl->name = identifier;
    decl->type = type;
    decl->is_final = is_final;
    decl->variable_index = -1;

    return decl;
}

PackageName *
zec_create_package_name(char *identifier)
{
    PackageName *pn;

    pn = zec_malloc(sizeof(PackageName));
    pn->name = identifier;
    pn->next = NULL;

    return pn;
}

PackageName *
zec_chain_package_name(PackageName *list, char *identifier)
{
    PackageName *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_package_name(identifier);

    return list;
}

RequireList *
zec_create_require_list(PackageName *package_name)
{
    RequireList *rl;
    ZEC_Compiler *compiler;
    char *current_package_name;
    char *req_package_name;

    compiler = zec_get_current_compiler();

    current_package_name = zec_package_name_to_string(compiler->package_name);
    req_package_name = zec_package_name_to_string(package_name);
    if (zvm_compare_string(req_package_name, current_package_name)
        && compiler->source_suffix == ZEH_SOURCE) {
        zec_compile_error(compiler->current_line_number,
                          REQUIRE_ITSELF_ERR, MESSAGE_ARGUMENT_END);
    }
    MEM_free(current_package_name);
    MEM_free(req_package_name);

    rl = zec_malloc(sizeof(RequireList));
    rl->package_name = package_name;
    rl->line_number = zec_get_current_compiler()->current_line_number;
    rl->next = NULL;

    return rl;
}

RequireList *
zec_chain_require_list(RequireList *list, RequireList *add)
{
    RequireList *pos;
    char buf[LINE_BUF_SIZE];

    for (pos = list; pos->next; pos = pos->next) {
        if (zec_compare_package_name(pos->package_name, add->package_name)) {
            char *package_name;
            package_name = zec_package_name_to_string(add->package_name);
            zvm_strncpy(buf, package_name, LINE_BUF_SIZE);
            MEM_free(package_name);
            zec_compile_error(zec_get_current_compiler()->current_line_number,
                              REQUIRE_DUPLICATE_ERR,
                              STRING_MESSAGE_ARGUMENT, "package_name", buf,
                              MESSAGE_ARGUMENT_END);
        }
    }
    pos->next = add;

    return list;
}

RenameList *
zec_create_rename_list(PackageName *package_name, char *identifier)
{
    RenameList *rl;
    PackageName *pre_tail;
    PackageName *tail;

    pre_tail = NULL;
    for (tail = package_name; tail->next; tail = tail->next) {
        pre_tail = tail;
    }
    if (pre_tail == NULL) {
        zec_compile_error(zec_get_current_compiler()->current_line_number,
                          RENAME_HAS_NO_PACKAGED_NAME_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    pre_tail->next = NULL;

    rl = zec_malloc(sizeof(RenameList));
    rl->package_name = package_name;
    rl->original_name = tail->name;
    rl->renamed_name = identifier;
    rl->line_number = zec_get_current_compiler()->current_line_number;
    rl->next = NULL;

    return rl;
}

RenameList *
zec_chain_rename_list(RenameList *list, RenameList *add)
{
    RenameList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = add;

    return list;
}

static RequireList *
add_default_package(RequireList *require_list)
{
    RequireList *req_pos;
    ZVM_Boolean default_package_required = ZVM_FALSE;

    for (req_pos = require_list; req_pos; req_pos = req_pos->next) {
        char *temp_name
            = zec_package_name_to_string(req_pos->package_name);
        if (!strcmp(temp_name, ZERO_DEFAULT_PACKAGE)) {
            default_package_required = ZVM_TRUE;
        }
        MEM_free(temp_name);
    }

    if (!default_package_required) {
        PackageName *pn;
        RequireList *req_temp;

        pn = zec_create_package_name(ZERO_DEFAULT_PACKAGE_P1);
        pn = zec_chain_package_name(pn, ZERO_DEFAULT_PACKAGE_P2);
        req_temp = require_list;
        require_list = zec_create_require_list(pn);
        require_list->next = req_temp;
    }
    return require_list;
}

void
zec_set_require_and_rename_list(RequireList *require_list,
                                RenameList *rename_list)
{
    ZEC_Compiler *compiler;
    char *current_package_name;

    compiler = zec_get_current_compiler();

    current_package_name
        = zec_package_name_to_string(compiler->package_name);

    if (!zvm_compare_string(current_package_name, ZERO_DEFAULT_PACKAGE)) {
        require_list = add_default_package(require_list);
    }
    MEM_free(current_package_name);
    compiler->require_list = require_list;
    compiler->rename_list = rename_list;
}

static void
add_function_to_compiler(FunctionDefinition *fd)
{
    ZEC_Compiler *compiler;
    FunctionDefinition *pos;

    compiler = zec_get_current_compiler();
    if (compiler->function_list) {
        for (pos = compiler->function_list; pos->next; pos = pos->next)
            ;
        pos->next = fd;
    } else {
        compiler->function_list = fd;
    }
}

FunctionDefinition *
zec_create_function_definition(TypeSpecifier *type, char *identifier,
                               ParameterList *parameter_list,
                               ExceptionList *throws, Block *block)
{
    FunctionDefinition *fd;
    ZEC_Compiler *compiler;

    compiler = zec_get_current_compiler();

    fd = zec_malloc(sizeof(FunctionDefinition));
    fd->type = type;
    fd->package_name = compiler->package_name;
    fd->name = identifier;
    fd->parameter = parameter_list;
    fd->block = block;
    fd->local_variable_count = 0;
    fd->local_variable = NULL;
    fd->class_definition = NULL;
    fd->throws = throws;
    fd->end_line_number = compiler->current_line_number;
    fd->next = NULL;
    if (block) {
        block->type = FUNCTION_BLOCK;
        block->parent.function.function = fd;
    }
    add_function_to_compiler(fd);

    return fd;
}

void
zec_function_define(TypeSpecifier *type, char *identifier,
                    ParameterList *parameter_list, ExceptionList *throws,
                    Block *block)
{
    FunctionDefinition *fd;

    if (zec_search_function(identifier)
        || zec_search_declaration(identifier, NULL)) {
        zec_compile_error(zec_get_current_compiler()->current_line_number,
                          FUNCTION_MULTIPLE_DEFINE_ERR,
                          STRING_MESSAGE_ARGUMENT, "name", identifier,
                          MESSAGE_ARGUMENT_END);
        return;
    }
    fd = zec_create_function_definition(type, identifier, parameter_list,
                                        throws, block);
}

ParameterList *
zec_create_parameter(TypeSpecifier *type, char *identifier)
{
    ParameterList       *p;

    p = zec_malloc(sizeof(ParameterList));
    p->name = identifier;
    p->type = type;
    p->line_number = zec_get_current_compiler()->current_line_number;
    p->next = NULL;

    return p;
}

ParameterList *
zec_chain_parameter(ParameterList *list, TypeSpecifier *type,
                    char *identifier)
{
    ParameterList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_parameter(type, identifier);

    return list;
}

ArgumentList *
zec_create_argument_list(Expression *expression)
{
    ArgumentList *al;

    al = zec_malloc(sizeof(ArgumentList));
    al->expression = expression;
    al->next = NULL;

    return al;
}

ArgumentList *
zec_chain_argument_list(ArgumentList *list, Expression *expr)
{
    ArgumentList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_argument_list(expr);

    return list;
}

ExpressionList *
zec_create_expression_list(Expression *expression)
{
    ExpressionList *el;

    el = zec_malloc(sizeof(ExpressionList));
    el->expression = expression;
    el->next = NULL;

    return el;
}

ExpressionList *
zec_chain_expression_list(ExpressionList *list, Expression *expr)
{
    ExpressionList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_expression_list(expr);

    return list;
}

StatementList *
zec_create_statement_list(Statement *statement)
{
    StatementList *sl;

    sl = zec_malloc(sizeof(StatementList));
    sl->statement = statement;
    sl->next = NULL;

    return sl;
}

StatementList *
zec_chain_statement_list(StatementList *list, Statement *statement)
{
    StatementList *pos;

    if (list == NULL)
        return zec_create_statement_list(statement);

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_statement_list(statement);

    return list;
}


TypeSpecifier *
zec_create_type_specifier(ZVM_BasicType basic_type)
{
    TypeSpecifier *type;

    type = zec_alloc_type_specifier(basic_type);
    type->line_number = zec_get_current_compiler()->current_line_number;

    return type;
}

TypeSpecifier *
zec_create_identifier_type_specifier(char *identifier)
{
    TypeSpecifier *type;

    type = zec_alloc_type_specifier(ZVM_UNSPECIFIED_IDENTIFIER_TYPE);
    type->identifier = identifier;
    type->line_number = zec_get_current_compiler()->current_line_number;

    return type;
}

TypeSpecifier *
zec_create_array_type_specifier(TypeSpecifier *base)
{
    TypeDerive *new_derive;
    
    new_derive = zec_alloc_type_derive(ARRAY_DERIVE);

    if (base->derive == NULL) {
        base->derive = new_derive;
    } else {
        TypeDerive *derive_p;
        for (derive_p = base->derive; derive_p->next != NULL;
             derive_p = derive_p->next)
            ;
        derive_p->next = new_derive;
    }

    return base;
}

Expression *
zec_alloc_expression(ExpressionKind kind)
{
    Expression  *exp;

    exp = zec_malloc(sizeof(Expression));
    exp->type = NULL;
    exp->kind = kind;
    exp->line_number = zec_get_current_compiler()->current_line_number;

    return exp;
}

Expression *
zec_create_comma_expression(Expression *left, Expression *right)
{
    Expression *exp;

    exp = zec_alloc_expression(COMMA_EXPRESSION);
    exp->u.comma.left = left;
    exp->u.comma.right = right;

    return exp;
}

Expression *
zec_create_assign_expression(Expression *left, AssignmentOperator operator,
                             Expression *operand)
{
    Expression *exp;

    exp = zec_alloc_expression(ASSIGN_EXPRESSION);
    exp->u.assign_expression.left = left;
    exp->u.assign_expression.operator = operator;
    exp->u.assign_expression.operand = operand;

    return exp;
}

Expression *
zec_create_binary_expression(ExpressionKind operator,
                             Expression *left, Expression *right)
{
#if 0
    if ((left->kind == INT_EXPRESSION
         || left->kind == DOUBLE_EXPRESSION)
        && (right->kind == INT_EXPRESSION
            || right->kind == DOUBLE_EXPRESSION)) {
        DKC_Value v;
        v = zec_eval_binary_expression(zec_get_current_compiler(),
                                       NULL, operator, left, right);
        /* Overwriting left hand expression. */
        *left = convert_value_to_expression(&v);

        return left;
    } else {
#endif
        Expression *exp;
        exp = zec_alloc_expression(operator);
        exp->u.binary_expression.left = left;
        exp->u.binary_expression.right = right;
        return exp;
#if 0
    }
#endif
}

Expression *
zec_create_minus_expression(Expression *operand)
{
#if 0
    if (operand->kind == INT_EXPRESSION
        || operand->kind == DOUBLE_EXPRESSION) {
        DKC_Value       v;
        v = zec_eval_minus_expression(zec_get_current_compiler(),
                                      NULL, operand);
        /* Notice! Overwriting operand expression. */
        *operand = convert_value_to_expression(&v);
        return operand;
    } else {
#endif
        Expression      *exp;
        exp = zec_alloc_expression(MINUS_EXPRESSION);
        exp->u.minus_expression = operand;
        return exp;
#if 0
    }
#endif
}

Expression *
zec_create_logical_not_expression(Expression *operand)
{
    Expression  *exp;

    exp = zec_alloc_expression(LOGICAL_NOT_EXPRESSION);
    exp->u.logical_not = operand;

    return exp;
}

Expression *
zec_create_index_expression(Expression *array, Expression *index)
{
    Expression *exp;

    exp = zec_alloc_expression(INDEX_EXPRESSION);
    exp->u.index_expression.array = array;
    exp->u.index_expression.index = index;

    return exp;
}

Expression *
zec_create_incdec_expression(Expression *operand, ExpressionKind inc_or_dec)
{
    Expression *exp;

    exp = zec_alloc_expression(inc_or_dec);
    exp->u.inc_dec.operand = operand;

    return exp;
}

Expression *
zec_create_instanceof_expression(Expression *operand, TypeSpecifier *type)
{
    Expression *exp;

    exp = zec_alloc_expression(INSTANCEOF_EXPRESSION);
    exp->u.instanceof.operand = operand;
    exp->u.instanceof.type = type;

    return exp;
}

Expression *
zec_create_identifier_expression(char *identifier)
{
    Expression  *exp;

    exp = zec_alloc_expression(IDENTIFIER_EXPRESSION);
    exp->u.identifier.name = identifier;

    return exp;
}

Expression *
zec_create_function_call_expression(Expression *function,
                                    ArgumentList *argument)
{
    Expression  *exp;

    exp = zec_alloc_expression(FUNCTION_CALL_EXPRESSION);
    exp->u.function_call_expression.function = function;
    exp->u.function_call_expression.argument = argument;

    return exp;
}

Expression *
zec_create_down_cast_expression(Expression *operand, TypeSpecifier *type)
{
    Expression  *exp;

    exp = zec_alloc_expression(DOWN_CAST_EXPRESSION);
    exp->u.down_cast.operand = operand;
    exp->u.down_cast.type = type;

    return exp;
}

Expression *
zec_create_member_expression(Expression *expression, char *member_name)
{
    Expression  *exp;

    exp = zec_alloc_expression(MEMBER_EXPRESSION);
    exp->u.member_expression.expression = expression;
    exp->u.member_expression.member_name = member_name;

    return exp;
}


Expression *
zec_create_boolean_expression(ZVM_Boolean value)
{
    Expression *exp;

    exp = zec_alloc_expression(BOOLEAN_EXPRESSION);
    exp->u.boolean_value = value;

    return exp;
}

Expression *
zec_create_null_expression(void)
{
    Expression  *exp;

    exp = zec_alloc_expression(NULL_EXPRESSION);

    return exp;
}

Expression *
zec_create_this_expression(void)
{
    Expression  *exp;

    exp = zec_alloc_expression(THIS_EXPRESSION);

    return exp;
}

Expression *
zec_create_super_expression(void)
{
    Expression  *exp;

    exp = zec_alloc_expression(SUPER_EXPRESSION);

    return exp;
}

Expression *
zec_create_new_expression(char *class_name, char *method_name,
                          ArgumentList *argument)
{
    Expression *exp;

    exp = zec_alloc_expression(NEW_EXPRESSION);
    exp->u.new_e.class_name = class_name;
    exp->u.new_e.class_definition = NULL;
    exp->u.new_e.method_name = method_name;
    exp->u.new_e.method_declaration = NULL;
    exp->u.new_e.argument = argument;

    return exp;
}

Expression *
zec_create_array_literal_expression(ExpressionList *list)
{
    Expression  *exp;

    exp = zec_alloc_expression(ARRAY_LITERAL_EXPRESSION);
    exp->u.array_literal = list;

    return exp;
}

Expression *
zec_create_basic_array_creation(ZVM_BasicType basic_type,
                                ArrayDimension *dim_expr_list,
                                ArrayDimension *dim_list)
{
    Expression  *exp;
    TypeSpecifier *type;

    type = zec_create_type_specifier(basic_type);
    exp = zec_create_class_array_creation(type, dim_expr_list, dim_list);

    return exp;
}

Expression *
zec_create_class_array_creation(TypeSpecifier *type,
                                ArrayDimension *dim_expr_list,
                                ArrayDimension *dim_list)
{
    Expression  *exp;

    exp = zec_alloc_expression(ARRAY_CREATION_EXPRESSION);
    exp->u.array_creation.type = type;
    exp->u.array_creation.dimension
        = zec_chain_array_dimension(dim_expr_list, dim_list);

    return exp;
}

ArrayDimension *
zec_create_array_dimension(Expression *expr)
{
    ArrayDimension *dim;

    dim = zec_malloc(sizeof(ArrayDimension));
    dim->expression = expr;
    dim->next = NULL;

    return dim;
}

ArrayDimension *
zec_chain_array_dimension(ArrayDimension *list, ArrayDimension *dim)
{
    ArrayDimension *pos;

    for (pos = list; pos->next != NULL; pos = pos->next)
        ;
    pos->next = dim;

    return list;
}

Statement *
zec_alloc_statement(StatementType type)
{
    Statement *st;

    st = zec_malloc(sizeof(Statement));
    st->type = type;
    st->line_number = zec_get_current_compiler()->current_line_number;

    return st;
}

Statement *
zec_create_if_statement(Expression *condition,
                        Block *then_block, Elsif *elsif_list,
                        Block *else_block)
{
    Statement *st;

    st = zec_alloc_statement(IF_STATEMENT);
    st->u.if_s.condition = condition;
    st->u.if_s.then_block = then_block;
    st->u.if_s.elsif_list = elsif_list;
    st->u.if_s.else_block = else_block;

    return st;
}

Elsif *
zec_chain_elsif_list(Elsif *list, Elsif *add)
{
    Elsif *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = add;

    return list;
}

Elsif *
zec_create_elsif(Expression *expr, Block *block)
{
    Elsif *ei;

    ei = zec_malloc(sizeof(Elsif));
    ei->condition = expr;
    ei->block = block;
    ei->next = NULL;

    return ei;
}

Statement *
zec_create_switch_statement(Expression *expression,
                            CaseList *case_list, Block *default_block)
{
    Statement *st;

    st = zec_alloc_statement(SWITCH_STATEMENT);
    st->u.switch_s.expression = expression;
    st->u.switch_s.case_list = case_list;
    st->u.switch_s.default_block = default_block;

    return st;
}

CaseList *
zec_create_one_case(ExpressionList *expression_list, Block *block)
{
    CaseList *case_list;

    case_list = zec_malloc(sizeof(CaseList));
    case_list->expression_list = expression_list;
    case_list->block = block;
    case_list->next = NULL;

    return case_list;
}

CaseList *
zec_chain_case(CaseList *list, CaseList *add)
{
    CaseList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = add;

    return list;
}

Statement *
zec_create_while_statement(char *label,
                           Expression *condition, Block *block)
{
    Statement *st;

    st = zec_alloc_statement(WHILE_STATEMENT);
    st->u.while_s.label = label;
    st->u.while_s.condition = condition;
    st->u.while_s.block = block;
    block->type = WHILE_STATEMENT_BLOCK;
    block->parent.statement.statement = st;

    return st;
}

Statement *
zec_create_for_statement(char *label, Expression *init, Expression *cond,
                         Expression *post, Block *block)
{
    Statement *st;

    st = zec_alloc_statement(FOR_STATEMENT);
    st->u.for_s.label = label;
    st->u.for_s.init = init;
    st->u.for_s.condition = cond;
    st->u.for_s.post = post;
    st->u.for_s.block = block;
    block->type = FOR_STATEMENT_BLOCK;
    block->parent.statement.statement = st;

    return st;
}

Statement *
zec_create_do_while_statement(char *label, Block *block,
                              Expression *condition)
{
    Statement *st;

    st = zec_alloc_statement(DO_WHILE_STATEMENT);
    st->u.do_while_s.label = label;
    st->u.do_while_s.block = block;
    st->u.do_while_s.condition = condition;
    block->type = DO_WHILE_STATEMENT_BLOCK;
    block->parent.statement.statement = st;

    return st;
}

Statement *
zec_create_foreach_statement(char *label, char *variable,
                             Expression *collection, Block *block)
{
    Statement *st;

    st = zec_alloc_statement(FOREACH_STATEMENT);
    st->u.foreach_s.label = label;
    st->u.foreach_s.variable = variable;
    st->u.foreach_s.collection = collection;
    st->u.for_s.block = block;

    return st;
}

Block *
zec_alloc_block(void)
{
    Block *new_block;

    new_block = zec_malloc(sizeof(Block));
    new_block->type = UNDEFINED_BLOCK;
    new_block->outer_block = NULL;
    new_block->statement_list = NULL;
    new_block->declaration_list = NULL;

    return new_block;
}

Block *
zec_open_block(void)
{
    Block *new_block;

    ZEC_Compiler *compiler = zec_get_current_compiler();
    new_block = zec_alloc_block();
    new_block->outer_block = compiler->current_block;
    compiler->current_block = new_block;

    return new_block;
}

Block *
zec_close_block(Block *block, StatementList *statement_list)
{
    ZEC_Compiler *compiler = zec_get_current_compiler();

    DBG_assert(block == compiler->current_block,
               ("block mismatch.\n"));
    block->statement_list = statement_list;
    compiler->current_block = block->outer_block;

    return block;
}

Statement *
zec_create_expression_statement(Expression *expression)
{
    Statement *st;

    st = zec_alloc_statement(EXPRESSION_STATEMENT);
    st->u.expression_s = expression;

    return st;
}

Statement *
zec_create_return_statement(Expression *expression)
{
    Statement *st;

    st = zec_alloc_statement(RETURN_STATEMENT);
    st->u.return_s.return_value = expression;

    return st;
}

Statement *
zec_create_break_statement(char *label)
{
    Statement *st;

    st = zec_alloc_statement(BREAK_STATEMENT);
    st->u.break_s.label = label;

    return st;
}

Statement *
zec_create_continue_statement(char *label)
{
    Statement *st;

    st = zec_alloc_statement(CONTINUE_STATEMENT);
    st->u.continue_s.label = label;

    return st;
}

Statement *
zec_create_try_statement(Block *try_block,
                         CatchClause *catch_clause,
                         Block *finally_block)
{
    Statement *st;

    st = zec_alloc_statement(TRY_STATEMENT);
    st->u.try_s.try_block = try_block;
    try_block->type = TRY_CLAUSE_BLOCK;
    st->u.try_s.catch_clause = catch_clause;
    if (finally_block) {
        finally_block->type = FINALLY_CLAUSE_BLOCK;
    }
    st->u.try_s.finally_block = finally_block;

    return st;
}

CatchClause *
zec_create_catch_clause(TypeSpecifier *type, char *variable_name,
                        Block *block)
{
    CatchClause *cc;

    cc = zec_malloc(sizeof(CatchClause));
    cc->type = type;
    cc->variable_name = variable_name;
    cc->block = block;
    block->type = CATCH_CLAUSE_BLOCK;
    cc->next = NULL;

    return cc;
}

CatchClause *
zec_start_catch_clause(void)
{
    CatchClause *cc;

    cc = zec_malloc(sizeof(CatchClause));
    cc->line_number = zec_get_current_compiler()->current_line_number;
    cc->next = NULL;

    return cc;
}

CatchClause *
zec_end_catch_clause(CatchClause *catch_clause, TypeSpecifier *type,
                     char *variable_name, Block *block)
{
    catch_clause->type = type;
    catch_clause->variable_name = variable_name;
    catch_clause->block = block;

    return catch_clause;
}

CatchClause *
zec_chain_catch_list(CatchClause *list, CatchClause *add)
{
    CatchClause *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = add;

    return list;
}


Statement *
zec_create_throw_statement(Expression *expression)
{
    Statement *st;

    st = zec_alloc_statement(THROW_STATEMENT);
    st->u.throw_s.exception = expression;

    return st;
}

Statement *
zec_create_declaration_statement(ZVM_Boolean is_final, TypeSpecifier *type,
                                 char *identifier,
                                 Expression *initializer)
{
    Statement *st;
    Declaration *decl;
    ZEC_Compiler *compiler = zec_get_current_compiler();

    if (is_final && initializer == NULL) {
        zec_compile_error(compiler->current_line_number,
                          FINAL_VARIABLE_WITHOUT_INITIALIZER_ERR,
                          STRING_MESSAGE_ARGUMENT, "name", identifier,
                          MESSAGE_ARGUMENT_END);
    }
    st = zec_alloc_statement(DECLARATION_STATEMENT);

    decl = zec_alloc_declaration(is_final, type, identifier);

    decl->initializer = initializer;

    st->u.declaration_s = decl;

    return st;
}

ZVM_AccessModifier
conv_access_modifier(ClassOrMemberModifierKind src)
{
    if (src == PUBLIC_MODIFIER) {
        return ZVM_PUBLIC_ACCESS;
    } else if (src == PRIVATE_MODIFIER) {
        return ZVM_PRIVATE_ACCESS;
    } else {
        DBG_assert(src == NOT_SPECIFIED_MODIFIER, ("src..%d\n", src));
        return ZVM_FILE_ACCESS;
    }
}

void
zec_start_class_definition(ClassOrMemberModifierList *modifier,
                           ZVM_ClassOrInterface class_or_interface,
                           char *identifier,
                           ExtendsList *extends)
{
    ClassDefinition *cd;
    ZEC_Compiler *compiler = zec_get_current_compiler();

    cd = zec_malloc(sizeof(ClassDefinition));

    cd->is_abstract = (class_or_interface == ZVM_INTERFACE_DEFINITION);
    cd->access_modifier = ZVM_FILE_ACCESS;
    if (modifier) {
        if (modifier->is_abstract == ABSTRACT_MODIFIER) {
            cd->is_abstract = ZVM_TRUE;
        }
        cd->access_modifier = conv_access_modifier(modifier->access_modifier);
    }
    cd->class_or_interface = class_or_interface;
    cd->package_name = compiler->package_name;
    cd->name = identifier;
    cd->extends = extends;
    cd->super_class = NULL;
    cd->interface_list = NULL;
    cd->member = NULL;
    cd->next = NULL;
    cd->line_number = compiler->current_line_number;

    DBG_assert(compiler->current_class_definition == NULL,
               ("current_class_definition is not NULL."));
    compiler->current_class_definition = cd;
}

void zec_class_define(MemberDeclaration *member_list)
{
    ZEC_Compiler *compiler;
    ClassDefinition *cd;
    ClassDefinition *pos;

    compiler = zec_get_current_compiler();
    cd = compiler->current_class_definition;
    DBG_assert(cd != NULL, ("current_class_definition is NULL."));

    if (compiler->class_definition_list == NULL) {
        compiler->class_definition_list = cd;
    } else {
        for (pos = compiler->class_definition_list; pos->next;
             pos = pos->next)
            ;
        pos->next = cd;
    }
    cd->member = member_list;
    compiler->current_class_definition = NULL;
}

ExtendsList *
zec_create_extends_list(char *identifier)
{
    ExtendsList *list;

    list = zec_malloc(sizeof(ExtendsList));
    list->identifier = identifier;
    list->class_definition = NULL;
    list->next = NULL;

    return list;
}

ExtendsList *
zec_chain_extends_list(ExtendsList *list, char *add)
{
    ExtendsList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_extends_list(add);

    return list;
}

ClassOrMemberModifierList
zec_create_class_or_member_modifier(ClassOrMemberModifierKind modifier)
{
    ClassOrMemberModifierList ret;

    ret.is_abstract = NOT_SPECIFIED_MODIFIER;
    ret.access_modifier = NOT_SPECIFIED_MODIFIER;
    ret.is_override = NOT_SPECIFIED_MODIFIER;
    ret.is_virtual = NOT_SPECIFIED_MODIFIER;

    switch (modifier) {
    case ABSTRACT_MODIFIER:
        ret.is_abstract = ABSTRACT_MODIFIER;
        break;
    case PUBLIC_MODIFIER:
        ret.access_modifier = PUBLIC_MODIFIER;
        break;
    case PRIVATE_MODIFIER:
        ret.access_modifier = PRIVATE_MODIFIER;
        break;
    case OVERRIDE_MODIFIER:
        ret.is_override = OVERRIDE_MODIFIER;
        break;
    case VIRTUAL_MODIFIER:
        ret.is_virtual = VIRTUAL_MODIFIER;
        break;
    case NOT_SPECIFIED_MODIFIER: /* FALLTHRU */
    default:
        DBG_assert(0, ("modifier..%d", modifier));
    }

    return ret;
}

ClassOrMemberModifierList
zec_chain_class_or_member_modifier(ClassOrMemberModifierList list,
                                   ClassOrMemberModifierList add)
{
    if (add.is_abstract != NOT_SPECIFIED_MODIFIER) {
        DBG_assert(add.is_abstract == ABSTRACT_MODIFIER,
                   ("add.is_abstract..%d", add.is_abstract));
        if (list.is_abstract != NOT_SPECIFIED_MODIFIER) {
            zec_compile_error(zec_get_current_compiler()->current_line_number,
                              ABSTRACT_MULTIPLE_SPECIFIED_ERR,
                              MESSAGE_ARGUMENT_END);
        }
        list.is_abstract = ABSTRACT_MODIFIER;

    } else if (add.access_modifier != NOT_SPECIFIED_MODIFIER) {
        DBG_assert(add.access_modifier == PUBLIC_MODIFIER
                   || add.access_modifier == PUBLIC_MODIFIER,
                   ("add.access_modifier..%d", add.access_modifier));
        if (list.access_modifier != NOT_SPECIFIED_MODIFIER) {
            zec_compile_error(zec_get_current_compiler()->current_line_number,
                              ACCESS_MODIFIER_MULTIPLE_SPECIFIED_ERR,
                              MESSAGE_ARGUMENT_END);
        }
        list.access_modifier = add.access_modifier;

    } else if (add.is_override != NOT_SPECIFIED_MODIFIER) {
        DBG_assert(add.is_override == OVERRIDE_MODIFIER,
                   ("add.is_override..%d", add.is_override));
        if (list.is_override != NOT_SPECIFIED_MODIFIER) {
            zec_compile_error(zec_get_current_compiler()->current_line_number,
                              OVERRIDE_MODIFIER_MULTIPLE_SPECIFIED_ERR,
                              MESSAGE_ARGUMENT_END);
        }
        list.is_override = add.is_override;
    } else if (add.is_virtual != NOT_SPECIFIED_MODIFIER) {
        DBG_assert(add.is_virtual == VIRTUAL_MODIFIER,
                   ("add.is_virtual..%d", add.is_virtual));
        if (list.is_virtual != NOT_SPECIFIED_MODIFIER) {
            zec_compile_error(zec_get_current_compiler()->current_line_number,
                              VIRTUAL_MODIFIER_MULTIPLE_SPECIFIED_ERR,
                              MESSAGE_ARGUMENT_END);
        }
        list.is_virtual = add.is_virtual;
    }
    return list;
}

MemberDeclaration *
zec_chain_member_declaration(MemberDeclaration *list, MemberDeclaration *add)
{
    MemberDeclaration *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = add;

    return list;
}

static MemberDeclaration *
alloc_member_declaration(MemberKind kind,
                         ClassOrMemberModifierList *modifier)
{
    MemberDeclaration *ret;

    ret = zec_malloc(sizeof(MemberDeclaration));
    ret->kind = kind;
    if (modifier) {
        ret->access_modifier = conv_access_modifier(modifier->access_modifier);
    } else {
        ret->access_modifier = ZVM_FILE_ACCESS;
    }
    ret->line_number = zec_get_current_compiler()->current_line_number;
    ret->next = NULL;

    return ret;
}

MemberDeclaration *
zec_create_method_member(ClassOrMemberModifierList *modifier,
                         FunctionDefinition *function_definition,
                         ZVM_Boolean is_constructor)
{
    MemberDeclaration *ret;
    ZEC_Compiler *compiler;

    ret = alloc_member_declaration(METHOD_MEMBER, modifier);
    ret->u.method.is_constructor = is_constructor;
    ret->u.method.is_abstract = ZVM_FALSE;
    ret->u.method.is_virtual = ZVM_FALSE;
    ret->u.method.is_override = ZVM_FALSE;
    if (modifier) {
        if (modifier->is_abstract == ABSTRACT_MODIFIER) {
            ret->u.method.is_abstract = ZVM_TRUE;
        }
        if (modifier->is_virtual == VIRTUAL_MODIFIER) {
            ret->u.method.is_virtual = ZVM_TRUE;
        }
        if (modifier->is_override == OVERRIDE_MODIFIER) {
            ret->u.method.is_override = ZVM_TRUE;
        }
    }
    compiler = zec_get_current_compiler();
    if (compiler->current_class_definition->class_or_interface
        == ZVM_INTERFACE_DEFINITION) {
        /* BUGBUG error check */
        ret->u.method.is_abstract = ZVM_TRUE;
        ret->u.method.is_virtual = ZVM_TRUE;
    }

    ret->u.method.function_definition = function_definition;

    if (ret->u.method.is_abstract) {
        if (function_definition->block) {
            zec_compile_error(compiler->current_line_number,
                              ABSTRACT_METHOD_HAS_BODY_ERR,
                              MESSAGE_ARGUMENT_END);
        }
    } else {
        if (function_definition->block == NULL) {
            zec_compile_error(compiler->current_line_number,
                              CONCRETE_METHOD_HAS_NO_BODY_ERR,
                              MESSAGE_ARGUMENT_END);
        }
    }
    function_definition->class_definition
        = compiler->current_class_definition;

    return ret;
}

FunctionDefinition *
zec_method_function_define(TypeSpecifier *type, char *identifier,
                           ParameterList *parameter_list,
                           ExceptionList *throws, Block *block)
{
    FunctionDefinition *fd;

    fd = zec_create_function_definition(type, identifier, parameter_list,
                                        throws, block);

    return fd;
}

FunctionDefinition *
zec_constructor_function_define(char *identifier,
                                ParameterList *parameter_list,
                                ExceptionList *throws, Block *block)
{
    FunctionDefinition *fd;
    TypeSpecifier *type;

    type = zec_create_type_specifier(ZVM_VOID_TYPE);
    fd = zec_method_function_define(type, identifier, parameter_list,
                                    throws, block);

    return fd;
}

MemberDeclaration *
zec_create_field_member(ClassOrMemberModifierList *modifier,
                        ZVM_Boolean is_final, TypeSpecifier *type, char *name)
{
    MemberDeclaration *ret;

    ret = alloc_member_declaration(FIELD_MEMBER, modifier);
    ret->u.field.name = name;
    ret->u.field.type = type;
    ret->u.field.is_final = is_final;

    return ret;
}

ExceptionList *
zec_create_throws(char *identifier)
{
    ExceptionList *list;

    list = zec_malloc(sizeof(ExceptionList));
    list->ref = zec_malloc(sizeof(ExceptionRef));
    list->ref->identifier = identifier;
    list->ref->class_definition = NULL;
    list->ref->line_number = zec_get_current_compiler()->current_line_number;
    list->next = NULL;

    return list;
}

ExceptionList *
zec_chain_exception_list(ExceptionList *list, char *identifier)
{
    ExceptionList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_throws(identifier);

    return list;
}

void
zec_create_delegate_definition(TypeSpecifier *type, char *identifier,
                               ParameterList *parameter_list,
                               ExceptionList *throws)
{
    DelegateDefinition *dd;
    DelegateDefinition *pos;
    ZEC_Compiler *compiler = zec_get_current_compiler();

    dd = zec_malloc(sizeof(DelegateDefinition));
    dd->type = type;
    dd->name = identifier;
    dd->parameter_list = parameter_list;
    dd->throws = throws;
    dd->next = NULL;

    if (compiler->delegate_definition_list == NULL) {
        compiler->delegate_definition_list = dd;
    } else {
        for (pos = compiler->delegate_definition_list; pos->next;
             pos = pos->next)
            ;
        pos->next = dd;
    }
}

void
zec_create_enum_definition(char *identifier, Enumerator *enumerator)
{
    EnumDefinition *ed;
    EnumDefinition *pos;
    ZEC_Compiler *compiler = zec_get_current_compiler();
    int value;
    Enumerator *enumerator_pos;

    ed = zec_malloc(sizeof(EnumDefinition));
    ed->name = identifier;
    ed->enumerator = enumerator;
    ed->next = NULL;

    value = 0;
    for (enumerator_pos = enumerator; enumerator_pos;
         enumerator_pos = enumerator_pos->next) {
        enumerator_pos->value = value;
        value++;
    }

    if (compiler->enum_definition_list == NULL) {
        compiler->enum_definition_list = ed;
    } else {
        for (pos = compiler->enum_definition_list; pos->next;
             pos = pos->next)
            ;
        pos->next = ed;
    }
}

Enumerator *
zec_create_enumerator(char *identifier)
{
    Enumerator *enumerator;

    enumerator = zec_malloc(sizeof(Enumerator));
    enumerator->name = identifier;
    enumerator->value = UNDEFINED_ENUMERATOR;
    enumerator->next = NULL;

    return enumerator;
}

Enumerator *
zec_chain_enumerator(Enumerator *enumerator, char *identifier)
{
    Enumerator *pos;

    for (pos = enumerator; pos->next; pos = pos->next)
        ;
    pos->next = zec_create_enumerator(identifier);

    return enumerator;
}
