/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_LITERAL = 258,
     DOUBLE_LITERAL = 259,
     STRING_LITERAL = 260,
     REGEXP_LITERAL = 261,
     IDENTIFIER = 262,
     IF = 263,
     ELSE = 264,
     ELSIF = 265,
     SWITCH = 266,
     CASE = 267,
     DEFAULT_T = 268,
     WHILE = 269,
     DO_T = 270,
     FOR = 271,
     FOREACH = 272,
     RETURN_T = 273,
     BREAK = 274,
     CONTINUE = 275,
     NULL_T = 276,
     LP = 277,
     RP = 278,
     LC = 279,
     RC = 280,
     LB = 281,
     RB = 282,
     SEMICOLON = 283,
     COLON = 284,
     COMMA = 285,
     ASSIGN_T = 286,
     LOGICAL_AND = 287,
     LOGICAL_OR = 288,
     EQ = 289,
     NE = 290,
     GT = 291,
     GE = 292,
     LT = 293,
     LE = 294,
     ADD = 295,
     SUB = 296,
     MUL = 297,
     DIV = 298,
     MOD = 299,
     TRUE_T = 300,
     FALSE_T = 301,
     EXCLAMATION = 302,
     DOT = 303,
     ADD_ASSIGN_T = 304,
     SUB_ASSIGN_T = 305,
     MUL_ASSIGN_T = 306,
     DIV_ASSIGN_T = 307,
     MOD_ASSIGN_T = 308,
     INCREMENT = 309,
     DECREMENT = 310,
     TRY = 311,
     CATCH = 312,
     FINALLY = 313,
     THROW = 314,
     THROWS = 315,
     VOID_T = 316,
     BOOLEAN_T = 317,
     INT_T = 318,
     DOUBLE_T = 319,
     STRING_T = 320,
     NATIVE_POINTER_T = 321,
     NEW = 322,
     REQUIRE = 323,
     RENAME = 324,
     CLASS_T = 325,
     INTERFACE_T = 326,
     PUBLIC_T = 327,
     PRIVATE_T = 328,
     VIRTUAL_T = 329,
     OVERRIDE_T = 330,
     ABSTRACT_T = 331,
     THIS_T = 332,
     SUPER_T = 333,
     CONSTRUCTOR = 334,
     INSTANCEOF = 335,
     DOWN_CAST_BEGIN = 336,
     DOWN_CAST_END = 337,
     DELEGATE = 338,
     FINAL = 339,
     ENUM = 340
   };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define STRING_LITERAL 260
#define REGEXP_LITERAL 261
#define IDENTIFIER 262
#define IF 263
#define ELSE 264
#define ELSIF 265
#define SWITCH 266
#define CASE 267
#define DEFAULT_T 268
#define WHILE 269
#define DO_T 270
#define FOR 271
#define FOREACH 272
#define RETURN_T 273
#define BREAK 274
#define CONTINUE 275
#define NULL_T 276
#define LP 277
#define RP 278
#define LC 279
#define RC 280
#define LB 281
#define RB 282
#define SEMICOLON 283
#define COLON 284
#define COMMA 285
#define ASSIGN_T 286
#define LOGICAL_AND 287
#define LOGICAL_OR 288
#define EQ 289
#define NE 290
#define GT 291
#define GE 292
#define LT 293
#define LE 294
#define ADD 295
#define SUB 296
#define MUL 297
#define DIV 298
#define MOD 299
#define TRUE_T 300
#define FALSE_T 301
#define EXCLAMATION 302
#define DOT 303
#define ADD_ASSIGN_T 304
#define SUB_ASSIGN_T 305
#define MUL_ASSIGN_T 306
#define DIV_ASSIGN_T 307
#define MOD_ASSIGN_T 308
#define INCREMENT 309
#define DECREMENT 310
#define TRY 311
#define CATCH 312
#define FINALLY 313
#define THROW 314
#define THROWS 315
#define VOID_T 316
#define BOOLEAN_T 317
#define INT_T 318
#define DOUBLE_T 319
#define STRING_T 320
#define NATIVE_POINTER_T 321
#define NEW 322
#define REQUIRE 323
#define RENAME 324
#define CLASS_T 325
#define INTERFACE_T 326
#define PUBLIC_T 327
#define PRIVATE_T 328
#define VIRTUAL_T 329
#define OVERRIDE_T 330
#define ABSTRACT_T 331
#define THIS_T 332
#define SUPER_T 333
#define CONSTRUCTOR 334
#define INSTANCEOF 335
#define DOWN_CAST_BEGIN 336
#define DOWN_CAST_END 337
#define DELEGATE 338
#define FINAL 339
#define ENUM 340




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 6 "zero.y"
typedef union YYSTYPE {
    char                *identifier;
    PackageName         *package_name;
    RequireList         *require_list;
    RenameList          *rename_list;
    ParameterList       *parameter_list;
    ArgumentList        *argument_list;
    Expression          *expression;
    ExpressionList      *expression_list;
    Statement           *statement;
    StatementList       *statement_list;
    Block               *block;
    Elsif               *elsif;
    CaseList            *case_list;
    CatchClause         *catch_clause;
    AssignmentOperator  assignment_operator;
    TypeSpecifier       *type_specifier;
    ZVM_BasicType       basic_type_specifier;
    ArrayDimension      *array_dimension;
    ClassOrMemberModifierList class_or_member_modifier;
    ZVM_ClassOrInterface class_or_interface;
    ExtendsList         *extends_list;
    MemberDeclaration   *member_declaration;
    FunctionDefinition  *function_definition;
    ExceptionList       *exception_list;
    Enumerator          *enumerator;
} YYSTYPE;
/* Line 1403 of yacc.c.  */
#line 236 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



