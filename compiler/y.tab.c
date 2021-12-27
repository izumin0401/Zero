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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "zero.y"

#include <stdio.h>
#include "zeroc.h"
#define YYDEBUG 1


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 191 of yacc.c.  */
#line 289 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 301 "y.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   845

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  85
/* YYNRULES -- Number of rules. */
#define YYNRULES  226
/* YYNRULES -- Number of states. */
#define YYNSTATES  421

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   340

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,     9,    10,    13,    15,    17,    19,
      22,    26,    28,    32,    34,    37,    42,    44,    46,    48,
      50,    52,    54,    56,    58,    60,    62,    64,    66,    70,
      74,    78,    80,    82,    84,    92,    99,   107,   114,   117,
     122,   124,   128,   130,   133,   135,   139,   141,   145,   147,
     149,   151,   153,   155,   157,   159,   163,   165,   169,   171,
     175,   179,   181,   185,   189,   193,   197,   199,   203,   207,
     209,   213,   217,   221,   223,   226,   229,   231,   234,   237,
     241,   243,   245,   247,   252,   257,   261,   266,   270,   274,
     279,   281,   283,   285,   287,   289,   291,   293,   295,   297,
     299,   304,   310,   317,   325,   329,   334,   338,   343,   347,
     352,   354,   357,   361,   364,   368,   369,   371,   375,   378,
     380,   382,   384,   386,   388,   390,   392,   394,   396,   398,
     400,   402,   408,   416,   423,   432,   434,   437,   443,   444,
     447,   454,   456,   459,   463,   464,   467,   469,   473,   480,
     491,   500,   509,   510,   512,   516,   517,   519,   523,   527,
     533,   538,   542,   544,   547,   548,   556,   560,   563,   567,
     573,   578,   585,   586,   591,   594,   595,   603,   604,   613,
     614,   621,   622,   630,   632,   635,   637,   639,   641,   643,
     645,   647,   648,   651,   653,   657,   659,   662,   664,   666,
     668,   671,   673,   676,   684,   691,   699,   706,   707,   710,
     712,   716,   724,   731,   739,   746,   748,   750,   754,   759,
     764,   770,   779,   787,   794,   802,   804
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
      87,     0,    -1,    88,    94,    -1,    87,    94,    -1,    -1,
      89,    92,    -1,    89,    -1,    92,    -1,    90,    -1,    89,
      90,    -1,    68,    91,    28,    -1,     7,    -1,    91,    48,
       7,    -1,    93,    -1,    92,    93,    -1,    69,    91,     7,
      28,    -1,    99,    -1,   149,    -1,   122,    -1,   168,    -1,
     169,    -1,    61,    -1,    62,    -1,    63,    -1,    64,    -1,
      65,    -1,    66,    -1,     7,    -1,    95,    26,    27,    -1,
       7,    26,    27,    -1,    97,    26,    27,    -1,    95,    -1,
      97,    -1,    96,    -1,    98,     7,    22,   100,    23,   163,
     147,    -1,    98,     7,    22,    23,   163,   147,    -1,    98,
       7,    22,   100,    23,   163,    28,    -1,    98,     7,    22,
      23,   163,    28,    -1,    98,     7,    -1,   100,    30,    98,
       7,    -1,   104,    -1,   101,    30,   104,    -1,   122,    -1,
     102,   122,    -1,   104,    -1,   103,    30,   104,    -1,   106,
      -1,   114,   105,   104,    -1,    31,    -1,    49,    -1,    50,
      -1,    51,    -1,    52,    -1,    53,    -1,   107,    -1,   106,
      33,   107,    -1,   108,    -1,   107,    32,   108,    -1,   109,
      -1,   108,    34,   109,    -1,   108,    35,   109,    -1,   110,
      -1,   109,    36,   110,    -1,   109,    37,   110,    -1,   109,
      38,   110,    -1,   109,    39,   110,    -1,   111,    -1,   110,
      40,   111,    -1,   110,    41,   111,    -1,   112,    -1,   111,
      42,   112,    -1,   111,    43,   112,    -1,   111,    44,   112,
      -1,   113,    -1,    41,   112,    -1,    47,   112,    -1,   114,
      -1,   114,    54,    -1,   114,    55,    -1,   114,    80,    98,
      -1,   115,    -1,   117,    -1,     7,    -1,   115,    26,   103,
      27,    -1,     7,    26,   103,    27,    -1,   114,    48,     7,
      -1,   114,    22,   101,    23,    -1,   114,    22,    23,    -1,
      22,   103,    23,    -1,   114,    81,    98,    82,    -1,     3,
      -1,     4,    -1,     5,    -1,     6,    -1,    45,    -1,    46,
      -1,    21,    -1,   116,    -1,    77,    -1,    78,    -1,    67,
       7,    22,    23,    -1,    67,     7,    22,   101,    23,    -1,
      67,     7,    48,     7,    22,    23,    -1,    67,     7,    48,
       7,    22,   101,    23,    -1,    24,   121,    25,    -1,    24,
     121,    30,    25,    -1,    67,    95,   118,    -1,    67,    95,
     118,   120,    -1,    67,    96,   118,    -1,    67,    96,   118,
     120,    -1,   119,    -1,   118,   119,    -1,    26,   103,    27,
      -1,    26,    27,    -1,   120,    26,    27,    -1,    -1,   104,
      -1,   121,    30,   104,    -1,   103,    28,    -1,   123,    -1,
     127,    -1,   132,    -1,   133,    -1,   134,    -1,   135,    -1,
     137,    -1,   139,    -1,   140,    -1,   141,    -1,   145,    -1,
     146,    -1,     8,    22,   103,    23,   147,    -1,     8,    22,
     103,    23,   147,     9,   147,    -1,     8,    22,   103,    23,
     147,   124,    -1,     8,    22,   103,    23,   147,   124,     9,
     147,    -1,   125,    -1,   124,   125,    -1,    10,    22,   103,
      23,   147,    -1,    -1,     7,    29,    -1,    11,    22,   103,
      23,   128,   130,    -1,   129,    -1,   128,   129,    -1,    12,
     131,   147,    -1,    -1,    13,   147,    -1,   104,    -1,   121,
      30,   104,    -1,   126,    14,    22,   103,    23,   147,    -1,
     126,    16,    22,   136,    28,   136,    28,   136,    23,   147,
      -1,   126,    15,   147,    14,    22,   103,    23,    28,    -1,
     126,    17,    22,     7,    29,   103,    23,   147,    -1,    -1,
     103,    -1,    18,   136,    28,    -1,    -1,     7,    -1,    19,
     138,    28,    -1,    20,   138,    28,    -1,    56,   147,   142,
      58,   147,    -1,    56,   147,    58,   147,    -1,    56,   147,
     142,    -1,   143,    -1,   142,   143,    -1,    -1,    57,   144,
      22,    98,     7,    23,   147,    -1,    59,   103,    28,    -1,
      59,    28,    -1,    98,     7,    28,    -1,    98,     7,    31,
     103,    28,    -1,    84,    98,     7,    28,    -1,    84,    98,
       7,    31,   103,    28,    -1,    -1,    24,   148,   102,    25,
      -1,    24,    25,    -1,    -1,   156,     7,   157,    24,   150,
     159,    25,    -1,    -1,   154,   156,     7,   157,    24,   151,
     159,    25,    -1,    -1,   156,     7,   157,    24,   152,    25,
      -1,    -1,   154,   156,     7,   157,    24,   153,    25,    -1,
     155,    -1,   154,   155,    -1,   166,    -1,    74,    -1,    75,
      -1,    76,    -1,    70,    -1,    71,    -1,    -1,    29,   158,
      -1,     7,    -1,   158,    30,     7,    -1,   160,    -1,   159,
     160,    -1,   161,    -1,   167,    -1,   162,    -1,   154,   162,
      -1,   165,    -1,   154,   165,    -1,    98,     7,    22,   100,
      23,   163,   147,    -1,    98,     7,    22,    23,   163,   147,
      -1,    98,     7,    22,   100,    23,   163,    28,    -1,    98,
       7,    22,    23,   163,    28,    -1,    -1,    60,   164,    -1,
       7,    -1,   164,    30,     7,    -1,    79,     7,    22,   100,
      23,   163,   147,    -1,    79,     7,    22,    23,   163,   147,
      -1,    79,     7,    22,   100,    23,   163,    28,    -1,    79,
       7,    22,    23,   163,    28,    -1,    72,    -1,    73,    -1,
      98,     7,    28,    -1,   154,    98,     7,    28,    -1,    84,
      98,     7,    28,    -1,   154,    84,    98,     7,    28,    -1,
      83,    98,     7,    22,   100,    23,   163,    28,    -1,    83,
      98,     7,    22,    23,   163,    28,    -1,    85,     7,    24,
     170,    25,    28,    -1,    85,     7,    24,   170,    30,    25,
      28,    -1,     7,    -1,   170,    30,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    90,    90,    91,    95,    98,   102,   106,   112,   113,
     119,   125,   129,   135,   136,   142,   148,   149,   150,   157,
     158,   161,   165,   169,   173,   177,   181,   187,   193,   199,
     205,   211,   215,   216,   219,   223,   227,   232,   238,   242,
     248,   252,   258,   262,   268,   269,   275,   276,   282,   286,
     290,   294,   298,   302,   308,   309,   315,   316,   322,   323,
     327,   333,   334,   338,   342,   346,   352,   353,   357,   363,
     364,   368,   372,   378,   379,   383,   389,   390,   394,   398,
     404,   405,   406,   412,   416,   421,   425,   429,   433,   437,
     441,   442,   443,   444,   445,   449,   453,   457,   458,   462,
     466,   470,   474,   478,   484,   488,   494,   498,   502,   506,
     513,   514,   520,   526,   530,   538,   541,   545,   551,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   569,   573,   577,   581,   587,   588,   594,   601,   604,
     610,   616,   617,   623,   630,   633,   639,   643,   649,   655,
     662,   668,   675,   678,   681,   688,   691,   694,   700,   706,
     710,   714,   720,   721,   728,   727,   737,   741,   747,   751,
     755,   759,   766,   765,   773,   782,   780,   791,   789,   798,
     797,   807,   805,   816,   817,   823,   824,   828,   832,   838,
     842,   849,   852,   858,   862,   868,   869,   875,   876,   879,
     883,   887,   891,   897,   901,   905,   910,   917,   920,   926,
     930,   936,   940,   944,   948,   954,   958,   964,   968,   973,
     977,   984,   989,   995,   999,  1005,  1009
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_LITERAL", "DOUBLE_LITERAL",
  "STRING_LITERAL", "REGEXP_LITERAL", "IDENTIFIER", "IF", "ELSE", "ELSIF",
  "SWITCH", "CASE", "DEFAULT_T", "WHILE", "DO_T", "FOR", "FOREACH",
  "RETURN_T", "BREAK", "CONTINUE", "NULL_T", "LP", "RP", "LC", "RC", "LB",
  "RB", "SEMICOLON", "COLON", "COMMA", "ASSIGN_T", "LOGICAL_AND",
  "LOGICAL_OR", "EQ", "NE", "GT", "GE", "LT", "LE", "ADD", "SUB", "MUL",
  "DIV", "MOD", "TRUE_T", "FALSE_T", "EXCLAMATION", "DOT", "ADD_ASSIGN_T",
  "SUB_ASSIGN_T", "MUL_ASSIGN_T", "DIV_ASSIGN_T", "MOD_ASSIGN_T",
  "INCREMENT", "DECREMENT", "TRY", "CATCH", "FINALLY", "THROW", "THROWS",
  "VOID_T", "BOOLEAN_T", "INT_T", "DOUBLE_T", "STRING_T",
  "NATIVE_POINTER_T", "NEW", "REQUIRE", "RENAME", "CLASS_T", "INTERFACE_T",
  "PUBLIC_T", "PRIVATE_T", "VIRTUAL_T", "OVERRIDE_T", "ABSTRACT_T",
  "THIS_T", "SUPER_T", "CONSTRUCTOR", "INSTANCEOF", "DOWN_CAST_BEGIN",
  "DOWN_CAST_END", "DELEGATE", "FINAL", "ENUM", "$accept",
  "translation_unit", "initial_declaration", "require_list",
  "require_declaration", "package_name", "rename_list",
  "rename_declaration", "definition_or_statement", "basic_type_specifier",
  "identifier_type_specifier", "array_type_specifier", "type_specifier",
  "function_definition", "parameter_list", "argument_list",
  "statement_list", "expression", "assignment_expression",
  "assignment_operator", "logical_or_expression", "logical_and_expression",
  "equality_expression", "relational_expression", "additive_expression",
  "multiplicative_expression", "unary_expression", "postfix_expression",
  "primary_expression", "primary_no_new_array", "array_literal",
  "array_creation", "dimension_expression_list", "dimension_expression",
  "dimension_list", "expression_list", "statement", "if_statement",
  "elsif_list", "elsif", "label_opt", "switch_statement", "case_list",
  "one_case", "default_opt", "case_expression_list", "while_statement",
  "for_statement", "do_while_statement", "foreach_statement",
  "expression_opt", "return_statement", "identifier_opt",
  "break_statement", "continue_statement", "try_statement", "catch_list",
  "catch_clause", "@1", "throw_statement", "declaration_statement",
  "block", "@2", "class_definition", "@3", "@4", "@5", "@6",
  "class_or_member_modifier_list", "class_or_member_modifier",
  "class_or_interface", "extends", "extends_list",
  "member_declaration_list", "member_declaration", "method_member",
  "method_function_definition", "throws_clause", "exception_list",
  "constructor_definition", "access_modifier", "field_member",
  "delegate_definition", "enum_definition", "enumerator_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    86,    87,    87,    88,    88,    88,    88,    89,    89,
      90,    91,    91,    92,    92,    93,    94,    94,    94,    94,
      94,    95,    95,    95,    95,    95,    95,    96,    97,    97,
      97,    98,    98,    98,    99,    99,    99,    99,   100,   100,
     101,   101,   102,   102,   103,   103,   104,   104,   105,   105,
     105,   105,   105,   105,   106,   106,   107,   107,   108,   108,
     108,   109,   109,   109,   109,   109,   110,   110,   110,   111,
     111,   111,   111,   112,   112,   112,   113,   113,   113,   113,
     114,   114,   114,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   116,   116,   117,   117,   117,   117,
     118,   118,   119,   120,   120,   121,   121,   121,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   123,   123,   123,   123,   124,   124,   125,   126,   126,
     127,   128,   128,   129,   130,   130,   131,   131,   132,   133,
     134,   135,   136,   136,   137,   138,   138,   139,   140,   141,
     141,   141,   142,   142,   144,   143,   145,   145,   146,   146,
     146,   146,   148,   147,   147,   150,   149,   151,   149,   152,
     149,   153,   149,   154,   154,   155,   155,   155,   155,   156,
     156,   157,   157,   158,   158,   159,   159,   160,   160,   161,
     161,   161,   161,   162,   162,   162,   162,   163,   163,   164,
     164,   165,   165,   165,   165,   166,   166,   167,   167,   167,
     167,   168,   168,   169,   169,   170,   170
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     2,     0,     2,     1,     1,     1,     2,
       3,     1,     3,     1,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     1,     1,     1,     7,     6,     7,     6,     2,     4,
       1,     3,     1,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     2,     2,     1,     2,     2,     3,
       1,     1,     1,     4,     4,     3,     4,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     5,     6,     7,     3,     4,     3,     4,     3,     4,
       1,     2,     3,     2,     3,     0,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     7,     6,     8,     1,     2,     5,     0,     2,
       6,     1,     2,     3,     0,     2,     1,     3,     6,    10,
       8,     8,     0,     1,     3,     0,     1,     3,     3,     5,
       4,     3,     1,     2,     0,     7,     3,     2,     3,     5,
       4,     6,     0,     4,     2,     0,     7,     0,     8,     0,
       6,     0,     7,     1,     2,     1,     1,     1,     1,     1,
       1,     0,     2,     1,     3,     1,     2,     1,     1,     1,
       2,     1,     2,     7,     6,     7,     6,     0,     2,     1,
       3,     7,     6,     7,     6,     1,     1,     3,     4,     4,
       5,     8,     7,     6,     7,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       4,     0,     0,   138,   138,     6,     8,     7,    13,    11,
       0,     0,     1,    90,    91,    92,    93,    82,     0,     0,
     152,   155,   155,    96,     0,   115,     0,    94,    95,     0,
       0,     0,    21,    22,    23,    24,    25,    26,     0,   189,
     190,   215,   216,   186,   187,   188,    98,    99,     0,     0,
       0,     3,    31,    33,    32,     0,    16,     0,    44,    46,
      54,    56,    58,    61,    66,    69,    73,    76,    80,    97,
      81,    18,   119,     0,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    17,     0,   183,     0,   185,
      19,    20,     2,     9,     5,    14,    10,     0,     0,     0,
     139,     0,     0,    82,   153,     0,   156,     0,     0,     0,
     116,     0,    74,    76,    75,   172,     0,   167,     0,    27,
       0,     0,    27,     0,     0,     0,     0,     0,     0,   118,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,    49,    50,    51,
      52,    53,    77,    78,     0,     0,     0,     0,     0,     0,
       0,     0,   184,     0,   191,    12,    15,    29,     0,     0,
       0,     0,   154,   157,   158,    88,   104,     0,   174,   138,
     164,     0,   161,   162,   166,     0,     0,     0,   106,   110,
     108,     0,     0,     0,     0,    28,    30,     0,   168,     0,
      45,    55,    57,    59,    60,    62,    63,    64,    65,    67,
      68,    70,    71,    72,    87,     0,    40,    85,    79,     0,
      47,     0,     0,     0,   152,     0,   191,     0,     0,    84,
       0,     0,   105,   117,     0,   138,    42,     0,   160,     0,
     163,   100,     0,     0,     0,     0,   111,   107,   109,     0,
     170,     0,   225,     0,   207,     0,     0,     0,    86,     0,
      89,    83,     0,     0,     0,     0,     0,   193,   192,   175,
     131,   115,   144,   141,     0,   173,    43,     0,   159,   101,
       0,   112,   113,     0,   207,     0,     0,     0,     0,     0,
       0,    38,   207,     0,   169,    41,     0,     0,   152,     0,
     177,     0,     0,     0,     0,     0,   133,   135,   116,     0,
       0,     0,   142,   140,     0,   102,     0,   114,     0,   207,
     171,   223,   226,     0,   209,   208,    37,    35,     0,     0,
     148,     0,     0,     0,     0,     0,   194,     0,     0,     0,
       0,     0,   195,   197,   199,   201,   198,   180,   132,     0,
       0,   136,     0,   143,   145,     0,   103,   222,     0,   224,
       0,    36,    34,    39,     0,   152,     0,     0,   182,     0,
       0,     0,     0,     0,   200,   202,   176,   196,     0,   134,
     117,     0,   221,   210,   150,     0,   151,   178,     0,     0,
       0,   217,     0,     0,     0,   165,     0,   207,     0,   219,
     207,     0,     0,   218,   137,   149,     0,   207,     0,   207,
     220,   214,   212,     0,   206,   204,     0,   213,   211,   205,
     203
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     3,     4,     5,     6,    10,     7,     8,    51,    52,
      53,    54,   255,    56,   256,   215,   235,    57,    58,   156,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,   188,   189,   247,   111,    71,    72,   306,   307,
      73,    74,   272,   273,   313,   310,    75,    76,    77,    78,
     105,    79,   107,    80,    81,    82,   182,   183,   237,    83,
      84,   116,   179,    85,   302,   334,   303,   335,   340,    87,
      88,   228,   268,   341,   342,   343,   344,   290,   325,   345,
      89,   346,    90,    91,   253
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -321
static const short int yypact[] =
{
     -41,    12,    12,   336,   419,   -41,  -321,    31,  -321,  -321,
      -6,     9,  -321,  -321,  -321,  -321,  -321,    33,    42,   114,
     752,   140,   140,  -321,   752,   752,   752,  -321,  -321,   752,
     133,   549,  -321,  -321,  -321,  -321,  -321,  -321,   161,  -321,
    -321,  -321,  -321,  -321,  -321,  -321,  -321,  -321,   309,   309,
     157,  -321,   182,  -321,   220,   191,  -321,    44,  -321,   221,
     227,   146,   199,   223,   130,  -321,  -321,   729,   240,  -321,
    -321,  -321,  -321,   234,  -321,  -321,  -321,  -321,  -321,  -321,
    -321,  -321,  -321,  -321,  -321,  -321,   562,  -321,   272,  -321,
    -321,  -321,  -321,  -321,    31,  -321,  -321,   282,   265,   578,
    -321,   752,   752,   274,   283,   289,  -321,   298,   299,    35,
    -321,   113,  -321,    68,  -321,   303,   215,  -321,    85,    -5,
     308,   308,   319,   322,   339,   325,   323,   324,     1,  -321,
     752,   752,   752,   752,   752,   752,   752,   752,   752,   752,
     752,   752,   752,   752,   607,  -321,   345,  -321,  -321,  -321,
    -321,  -321,  -321,  -321,   309,   309,   752,   752,   331,   133,
     337,   340,  -321,   354,   334,  -321,  -321,  -321,    26,    37,
      38,   752,  -321,  -321,  -321,  -321,  -321,   636,  -321,   502,
    -321,   133,   218,  -321,  -321,   665,   358,   752,   341,  -321,
     341,   349,   344,    73,   361,  -321,  -321,   148,  -321,   752,
    -321,   227,   146,   199,   199,   223,   223,   223,   223,   130,
     130,  -321,  -321,  -321,  -321,    40,  -321,  -321,  -321,   296,
    -321,   176,   752,   365,   752,   373,   334,   377,   362,  -321,
     133,   375,  -321,  -321,   378,    30,  -321,   366,  -321,   133,
    -321,  -321,    43,   367,   212,   694,  -321,   368,   368,   155,
    -321,   752,  -321,   131,   333,   384,    57,   174,  -321,   752,
    -321,  -321,    76,   374,   376,   386,   381,  -321,   387,   391,
     268,   752,   279,  -321,   213,  -321,  -321,   309,  -321,  -321,
     723,  -321,  -321,   401,   333,    79,   203,   390,    19,   422,
      -4,  -321,   333,   309,  -321,  -321,   133,   752,   752,   752,
     406,   425,   383,   408,   133,   412,   293,  -321,   411,   420,
     133,   133,  -321,  -321,   429,  -321,    80,  -321,   414,   333,
    -321,  -321,  -321,   423,  -321,   424,  -321,  -321,   139,   445,
    -321,    89,   433,    98,   383,   428,  -321,   456,   309,   461,
     466,   222,  -321,  -321,  -321,  -321,  -321,  -321,  -321,   752,
     133,  -321,   752,  -321,  -321,   446,  -321,  -321,   442,  -321,
     464,  -321,  -321,  -321,   444,   752,   133,   246,  -321,   452,
     469,     3,   309,   470,  -321,  -321,  -321,  -321,   103,  -321,
     455,   133,  -321,  -321,  -321,   465,  -321,  -321,   742,   459,
     779,  -321,   491,    96,   133,  -321,   133,   333,   111,  -321,
     333,   112,   471,  -321,  -321,  -321,   141,   333,   151,   333,
    -321,  -321,  -321,   172,  -321,  -321,   173,  -321,  -321,  -321,
    -321
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -321,  -321,  -321,  -321,   495,   499,   506,     4,   508,   476,
     477,  -321,    -3,  -321,  -244,  -175,  -321,   -17,   -19,  -321,
    -321,   385,   393,   171,   132,   175,   -11,  -321,    52,  -321,
    -321,  -321,   396,    70,   328,   248,  -166,  -321,  -321,   228,
    -321,  -321,  -321,   261,  -321,  -321,  -321,  -321,  -321,  -321,
    -215,  -321,   513,  -321,  -321,  -321,  -321,   355,  -321,  -321,
    -321,  -151,  -321,  -321,  -321,  -321,  -321,  -321,   320,   -84,
     450,   318,  -321,   217,  -320,  -321,   206,  -280,  -321,   219,
    -321,  -321,  -321,  -321,  -321
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -182
static const short int yytable[] =
{
      55,    55,   162,   104,   318,   285,   110,   109,   223,   264,
     242,    95,   328,   236,   118,   112,    98,   185,   114,     9,
     115,   377,    96,   197,   326,   390,   322,     1,     2,   198,
     238,   391,   199,    13,    14,    15,    16,    17,    18,   358,
     -27,    19,    97,   186,   323,   123,   124,   377,    20,    21,
      22,    23,    24,   229,    25,   275,   130,    97,   175,    99,
     230,   231,   100,   258,   101,   130,   279,   130,   130,   276,
     259,    26,   129,   259,   130,    27,    28,    29,   113,   270,
     292,   113,   168,   332,   169,   170,    30,   293,   278,    31,
     144,    32,    33,    34,    35,    36,    37,    38,    95,   296,
       2,   250,   319,   356,   251,   316,   130,    46,    47,   293,
     259,   200,   364,   184,    49,   130,   146,   406,   390,   130,
     408,   366,   152,   153,   403,   216,   394,   413,   130,   416,
     211,   212,   213,   130,   407,   409,   102,   220,   176,   327,
     221,   293,   293,   177,   398,   330,   401,   106,   154,   155,
     385,   218,   219,   348,   168,   122,   287,   115,   233,   353,
     354,   288,   122,   115,   125,   115,   216,   361,   119,   411,
     244,   254,   141,   142,   143,   115,   234,   362,   284,   414,
     133,   134,   257,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   115,   115,   128,   379,
     417,   419,   294,   261,   130,   262,   130,   104,   126,    32,
      33,    34,    35,    36,    37,   386,    32,    33,    34,    35,
      36,    37,    32,    33,    34,    35,    36,    37,   244,   122,
     395,   320,   234,   130,   286,   135,   136,   137,   138,   281,
     295,   198,   130,   404,   199,   405,   127,   376,   158,   159,
     160,   161,   308,   122,   131,   412,   162,   415,   246,   132,
     246,   216,   418,   139,   140,   420,   157,   205,   206,   207,
     208,   387,   180,   181,   314,   180,   239,   304,   305,   164,
     331,   104,   333,    32,    33,    34,    35,    36,    37,   165,
     329,   271,   311,   166,    41,    42,    43,    44,    45,   339,
     171,   337,   350,   305,   203,   204,   338,    32,    33,    34,
      35,    36,    37,   130,   209,   210,   122,   172,    41,    42,
      43,    44,    45,    86,    86,   337,   173,   174,   178,   192,
     338,   339,   378,   380,   187,   370,    12,   373,   339,    13,
      14,    15,    16,    17,    18,   191,   193,    19,   104,   194,
     195,   196,   217,   222,    20,    21,    22,    23,    24,   224,
      25,   226,   225,   227,   339,   243,   249,   245,   252,   392,
      32,    33,    34,    35,    36,    37,   167,    26,   260,   263,
     265,    27,    28,    29,   267,   274,   269,   271,   277,   280,
     122,   291,    30,   289,   283,    31,   297,    32,    33,    34,
      35,    36,    37,    38,   298,   300,    39,    40,    41,    42,
      43,    44,    45,    46,    47,   299,  -179,   301,   321,    48,
      49,    50,    13,    14,    15,    16,    17,    18,   317,   324,
      19,  -181,   336,   347,   349,  -146,   355,    20,    21,    22,
      23,    24,   357,    25,    32,    33,    34,    35,    36,    37,
     352,   359,   363,   368,   360,    41,    42,    43,    44,    45,
      26,   365,   337,   369,    27,    28,    29,   338,   371,   381,
     382,   383,   384,   122,   388,    30,   389,   393,    31,  -147,
      32,    33,    34,    35,    36,    37,    38,   399,   396,    39,
      40,    41,    42,    43,    44,    45,    46,    47,   402,   410,
      93,    11,    48,    49,    50,    13,    14,    15,    16,    17,
      18,    94,    92,    19,   120,   121,   201,   190,   248,   309,
      20,    21,    22,    23,    24,   202,    25,    32,    33,    34,
      35,    36,    37,   312,   351,   108,   163,   240,    41,    42,
      43,    44,    45,    26,   266,   337,   374,    27,    28,    29,
     372,   367,    13,    14,    15,    16,   103,     0,    30,   375,
       0,    31,     0,    32,    33,    34,    35,    36,    37,    38,
      23,    24,     0,    25,     0,     0,     0,   117,     0,    46,
      47,    13,    14,    15,    16,   103,    49,     0,     0,     0,
      26,     0,     0,     0,    27,    28,    29,     0,     0,    23,
      24,     0,    25,     0,     0,   167,     0,     0,     0,     0,
      13,    14,    15,    16,   103,     0,    38,     0,     0,    26,
       0,     0,     0,    27,    28,    29,    46,    47,    23,    24,
     214,    25,    39,    40,    41,    42,    43,    44,    45,    13,
      14,    15,    16,   103,     0,    38,     0,     0,    26,     0,
       0,     0,    27,    28,    29,    46,    47,    23,    24,     0,
      25,   232,     0,     0,     0,     0,     0,     0,    13,    14,
      15,    16,   103,     0,    38,     0,     0,    26,     0,     0,
       0,    27,    28,    29,    46,    47,    23,    24,   241,    25,
       0,     0,     0,     0,     0,     0,     0,    13,    14,    15,
      16,   103,     0,    38,     0,     0,    26,     0,     0,     0,
      27,    28,    29,    46,    47,    23,    24,     0,    25,     0,
       0,   282,     0,     0,     0,     0,    13,    14,    15,    16,
     103,     0,    38,     0,     0,    26,     0,     0,     0,    27,
      28,    29,    46,    47,    23,    24,   315,    25,     0,   122,
       0,   144,     0,     0,     0,    13,    14,    15,    16,   103,
     145,    38,     0,     0,    26,   397,     0,     0,    27,    28,
      29,    46,    47,    23,    24,     0,    25,   146,   147,   148,
     149,   150,   151,   152,   153,     0,   122,     0,     0,     0,
      38,     0,     0,    26,     0,     0,     0,    27,    28,    29,
      46,    47,   400,    32,    33,    34,    35,    36,    37,   154,
     155,     0,     0,     0,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,    35,    36,    37
};

static const short int yycheck[] =
{
       3,     4,    86,    20,   284,   249,    25,    24,   159,   224,
     185,     7,   292,   179,    31,    26,     7,    22,    29,     7,
      24,   341,    28,    22,    28,    22,     7,    68,    69,    28,
     181,    28,    31,     3,     4,     5,     6,     7,     8,   319,
       7,    11,    48,    48,    25,    48,    49,   367,    18,    19,
      20,    21,    22,    27,    24,    25,    30,    48,    23,    26,
      23,    23,    29,    23,    22,    30,    23,    30,    30,   235,
      30,    41,    28,    30,    30,    45,    46,    47,    26,   230,
      23,    29,    99,   298,   101,   102,    56,    30,   239,    59,
      22,    61,    62,    63,    64,    65,    66,    67,    94,    23,
      69,    28,    23,    23,    31,   280,    30,    77,    78,    30,
      30,   130,    23,    28,    84,    30,    48,   397,    22,    30,
     400,    23,    54,    55,    28,   144,    23,   407,    30,   409,
     141,   142,   143,    30,    23,    23,    22,   156,    25,   290,
     157,    30,    30,    30,   388,   296,   390,     7,    80,    81,
     365,   154,   155,   304,   171,     7,    25,    24,   177,   310,
     311,    30,     7,    24,     7,    24,   185,    28,     7,    28,
     187,    23,    42,    43,    44,    24,   179,   328,    23,    28,
      34,    35,   199,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,    24,    24,     7,   350,
      28,    28,    28,    27,    30,   222,    30,   224,    26,    61,
      62,    63,    64,    65,    66,   366,    61,    62,    63,    64,
      65,    66,    61,    62,    63,    64,    65,    66,   245,     7,
     381,    28,   235,    30,   251,    36,    37,    38,    39,    27,
     259,    28,    30,   394,    31,   396,    26,    25,    14,    15,
      16,    17,   271,     7,    33,   406,   340,   408,   188,    32,
     190,   280,   413,    40,    41,   416,    26,   135,   136,   137,
     138,    25,    57,    58,   277,    57,    58,     9,    10,     7,
     297,   298,   299,    61,    62,    63,    64,    65,    66,     7,
     293,    12,    13,    28,    72,    73,    74,    75,    76,   302,
      26,    79,     9,    10,   133,   134,    84,    61,    62,    63,
      64,    65,    66,    30,   139,   140,     7,    28,    72,    73,
      74,    75,    76,     3,     4,    79,    28,    28,    25,     7,
      84,   334,   349,   352,    26,   338,     0,   340,   341,     3,
       4,     5,     6,     7,     8,    26,     7,    11,   365,    24,
      27,    27,     7,    22,    18,    19,    20,    21,    22,    22,
      24,     7,    22,    29,   367,     7,    22,    26,     7,   372,
      61,    62,    63,    64,    65,    66,    27,    41,    82,    14,
       7,    45,    46,    47,     7,     7,    24,    12,    22,    22,
       7,     7,    56,    60,    26,    59,    22,    61,    62,    63,
      64,    65,    66,    67,    28,    24,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    29,    25,    30,    28,    83,
      84,    85,     3,     4,     5,     6,     7,     8,    27,     7,
      11,    25,     7,    25,    22,    24,     7,    18,    19,    20,
      21,    22,    28,    24,    61,    62,    63,    64,    65,    66,
      30,    28,     7,    25,    30,    72,    73,    74,    75,    76,
      41,    28,    79,     7,    45,    46,    47,    84,     7,    23,
      28,     7,    28,     7,    22,    56,     7,     7,    59,    24,
      61,    62,    63,    64,    65,    66,    67,    28,    23,    70,
      71,    72,    73,    74,    75,    76,    77,    78,     7,    28,
       5,     2,    83,    84,    85,     3,     4,     5,     6,     7,
       8,     5,     4,    11,    38,    38,   131,   121,   190,   271,
      18,    19,    20,    21,    22,   132,    24,    61,    62,    63,
      64,    65,    66,   272,   306,    22,    86,   182,    72,    73,
      74,    75,    76,    41,   226,    79,   340,    45,    46,    47,
      84,   334,     3,     4,     5,     6,     7,    -1,    56,   340,
      -1,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      21,    22,    -1,    24,    -1,    -1,    -1,    28,    -1,    77,
      78,     3,     4,     5,     6,     7,    84,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    45,    46,    47,    -1,    -1,    21,
      22,    -1,    24,    -1,    -1,    27,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    67,    -1,    -1,    41,
      -1,    -1,    -1,    45,    46,    47,    77,    78,    21,    22,
      23,    24,    70,    71,    72,    73,    74,    75,    76,     3,
       4,     5,     6,     7,    -1,    67,    -1,    -1,    41,    -1,
      -1,    -1,    45,    46,    47,    77,    78,    21,    22,    -1,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    67,    -1,    -1,    41,    -1,    -1,
      -1,    45,    46,    47,    77,    78,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    67,    -1,    -1,    41,    -1,    -1,    -1,
      45,    46,    47,    77,    78,    21,    22,    -1,    24,    -1,
      -1,    27,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    67,    -1,    -1,    41,    -1,    -1,    -1,    45,
      46,    47,    77,    78,    21,    22,    23,    24,    -1,     7,
      -1,    22,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      31,    67,    -1,    -1,    41,    23,    -1,    -1,    45,    46,
      47,    77,    78,    21,    22,    -1,    24,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,     7,    -1,    -1,    -1,
      67,    -1,    -1,    41,    -1,    -1,    -1,    45,    46,    47,
      77,    78,    23,    61,    62,    63,    64,    65,    66,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,    66
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    68,    69,    87,    88,    89,    90,    92,    93,     7,
      91,    91,     0,     3,     4,     5,     6,     7,     8,    11,
      18,    19,    20,    21,    22,    24,    41,    45,    46,    47,
      56,    59,    61,    62,    63,    64,    65,    66,    67,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    83,    84,
      85,    94,    95,    96,    97,    98,    99,   103,   104,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   122,   123,   126,   127,   132,   133,   134,   135,   137,
     139,   140,   141,   145,   146,   149,   154,   155,   156,   166,
     168,   169,    94,    90,    92,    93,    28,    48,     7,    26,
      29,    22,    22,     7,   103,   136,     7,   138,   138,   103,
     104,   121,   112,   114,   112,    24,   147,    28,   103,     7,
      95,    96,     7,    98,    98,     7,    26,    26,     7,    28,
      30,    33,    32,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    22,    31,    48,    49,    50,    51,
      52,    53,    54,    55,    80,    81,   105,    26,    14,    15,
      16,    17,   155,   156,     7,     7,    28,    27,   103,   103,
     103,    26,    28,    28,    28,    23,    25,    30,    25,   148,
      57,    58,   142,   143,    28,    22,    48,    26,   118,   119,
     118,    26,     7,     7,    24,    27,    27,    22,    28,    31,
     104,   107,   108,   109,   109,   110,   110,   110,   110,   111,
     111,   112,   112,   112,    23,   101,   104,     7,    98,    98,
     104,   103,    22,   147,    22,    22,     7,    29,   157,    27,
      23,    23,    25,   104,    98,   102,   122,   144,   147,    58,
     143,    23,   101,     7,   103,    26,   119,   120,   120,    22,
      28,    31,     7,   170,    23,    98,   100,   103,    23,    30,
      82,    27,   103,    14,   136,     7,   157,     7,   158,    24,
     147,    12,   128,   129,     7,    25,   122,    22,   147,    23,
      22,    27,    27,    26,    23,   100,   103,    25,    30,    60,
     163,     7,    23,    30,    28,   104,    23,    22,    28,    29,
      24,    30,   150,   152,     9,    10,   124,   125,   104,   121,
     131,    13,   129,   130,    98,    23,   101,    27,   163,    23,
      28,    28,     7,    25,     7,   164,    28,   147,   163,    98,
     147,   103,   136,   103,   151,   153,     7,    79,    84,    98,
     154,   159,   160,   161,   162,   165,   167,    25,   147,    22,
       9,   125,    30,   147,   147,     7,    23,    28,   163,    28,
      30,    28,   147,     7,    23,    28,    23,   159,    25,     7,
      98,     7,    84,    98,   162,   165,    25,   160,   103,   147,
     104,    23,    28,     7,    28,   136,   147,    25,    22,     7,
      22,    28,    98,     7,    23,   147,    23,    23,   100,    28,
      23,   100,     7,    28,   147,   147,   163,    23,   163,    23,
      28,    28,   147,   163,    28,   147,   163,    28,   147,    28,
     147
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 95 "zero.y"
    {
            zec_set_require_and_rename_list(NULL, NULL);
        }
    break;

  case 5:
#line 99 "zero.y"
    {
            zec_set_require_and_rename_list((yyvsp[-1].require_list), (yyvsp[0].rename_list));
        }
    break;

  case 6:
#line 103 "zero.y"
    {
            zec_set_require_and_rename_list((yyvsp[0].require_list), NULL);
        }
    break;

  case 7:
#line 107 "zero.y"
    {
            zec_set_require_and_rename_list(NULL, (yyvsp[0].rename_list));
        }
    break;

  case 9:
#line 114 "zero.y"
    {
            (yyval.require_list) = zec_chain_require_list((yyvsp[-1].require_list), (yyvsp[0].require_list));
        }
    break;

  case 10:
#line 120 "zero.y"
    {
            (yyval.require_list) = zec_create_require_list((yyvsp[-1].package_name));
        }
    break;

  case 11:
#line 126 "zero.y"
    {
            (yyval.package_name) = zec_create_package_name((yyvsp[0].identifier));
        }
    break;

  case 12:
#line 130 "zero.y"
    {
            (yyval.package_name) = zec_chain_package_name((yyvsp[-2].package_name), (yyvsp[0].identifier));
        }
    break;

  case 14:
#line 137 "zero.y"
    {
            (yyval.rename_list) = zec_chain_rename_list((yyvsp[-1].rename_list), (yyvsp[0].rename_list));
        }
    break;

  case 15:
#line 143 "zero.y"
    {
            (yyval.rename_list) = zec_create_rename_list((yyvsp[-2].package_name), (yyvsp[-1].identifier));
        }
    break;

  case 18:
#line 151 "zero.y"
    {
            ZEC_Compiler *compiler = zec_get_current_compiler();

            compiler->statement_list
                = zec_chain_statement_list(compiler->statement_list, (yyvsp[0].statement));
        }
    break;

  case 21:
#line 162 "zero.y"
    {
            (yyval.basic_type_specifier) = ZVM_VOID_TYPE;
        }
    break;

  case 22:
#line 166 "zero.y"
    {
            (yyval.basic_type_specifier) = ZVM_BOOLEAN_TYPE;
        }
    break;

  case 23:
#line 170 "zero.y"
    {
            (yyval.basic_type_specifier) = ZVM_INT_TYPE;
        }
    break;

  case 24:
#line 174 "zero.y"
    {
            (yyval.basic_type_specifier) = ZVM_DOUBLE_TYPE;
        }
    break;

  case 25:
#line 178 "zero.y"
    {
            (yyval.basic_type_specifier) = ZVM_STRING_TYPE;
        }
    break;

  case 26:
#line 182 "zero.y"
    {
            (yyval.basic_type_specifier) = ZVM_NATIVE_POINTER_TYPE;
        }
    break;

  case 27:
#line 188 "zero.y"
    {
            (yyval.type_specifier) = zec_create_identifier_type_specifier((yyvsp[0].identifier));
        }
    break;

  case 28:
#line 194 "zero.y"
    {
            TypeSpecifier *basic_type
                = zec_create_type_specifier((yyvsp[-2].basic_type_specifier));
            (yyval.type_specifier) = zec_create_array_type_specifier(basic_type);
        }
    break;

  case 29:
#line 200 "zero.y"
    {
            TypeSpecifier *identifier_type
                = zec_create_identifier_type_specifier((yyvsp[-2].identifier));
            (yyval.type_specifier) = zec_create_array_type_specifier(identifier_type);
        }
    break;

  case 30:
#line 206 "zero.y"
    {
            (yyval.type_specifier) = zec_create_array_type_specifier((yyvsp[-2].type_specifier));
        }
    break;

  case 31:
#line 212 "zero.y"
    {
                    (yyval.type_specifier) = zec_create_type_specifier((yyvsp[0].basic_type_specifier));
        }
    break;

  case 34:
#line 220 "zero.y"
    {
            zec_function_define((yyvsp[-6].type_specifier), (yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list), (yyvsp[0].block));
        }
    break;

  case 35:
#line 224 "zero.y"
    {
            zec_function_define((yyvsp[-5].type_specifier), (yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list), (yyvsp[0].block));
        }
    break;

  case 36:
#line 229 "zero.y"
    {
            zec_function_define((yyvsp[-6].type_specifier), (yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list), NULL);
        }
    break;

  case 37:
#line 233 "zero.y"
    {
            zec_function_define((yyvsp[-5].type_specifier), (yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list), NULL);
        }
    break;

  case 38:
#line 239 "zero.y"
    {
            (yyval.parameter_list) = zec_create_parameter((yyvsp[-1].type_specifier), (yyvsp[0].identifier));
        }
    break;

  case 39:
#line 243 "zero.y"
    {
            (yyval.parameter_list) = zec_chain_parameter((yyvsp[-3].parameter_list), (yyvsp[-1].type_specifier), (yyvsp[0].identifier));
        }
    break;

  case 40:
#line 249 "zero.y"
    {
            (yyval.argument_list) = zec_create_argument_list((yyvsp[0].expression));
        }
    break;

  case 41:
#line 253 "zero.y"
    {
            (yyval.argument_list) = zec_chain_argument_list((yyvsp[-2].argument_list), (yyvsp[0].expression));
        }
    break;

  case 42:
#line 259 "zero.y"
    {
            (yyval.statement_list) = zec_create_statement_list((yyvsp[0].statement));
        }
    break;

  case 43:
#line 263 "zero.y"
    {
            (yyval.statement_list) = zec_chain_statement_list((yyvsp[-1].statement_list), (yyvsp[0].statement));
        }
    break;

  case 45:
#line 270 "zero.y"
    {
            (yyval.expression) = zec_create_comma_expression((yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 47:
#line 277 "zero.y"
    {
            (yyval.expression) = zec_create_assign_expression((yyvsp[-2].expression), (yyvsp[-1].assignment_operator), (yyvsp[0].expression));
        }
    break;

  case 48:
#line 283 "zero.y"
    {
            (yyval.assignment_operator) = NORMAL_ASSIGN;
        }
    break;

  case 49:
#line 287 "zero.y"
    {
            (yyval.assignment_operator) = ADD_ASSIGN;
        }
    break;

  case 50:
#line 291 "zero.y"
    {
            (yyval.assignment_operator) = SUB_ASSIGN;
        }
    break;

  case 51:
#line 295 "zero.y"
    {
            (yyval.assignment_operator) = MUL_ASSIGN;
        }
    break;

  case 52:
#line 299 "zero.y"
    {
            (yyval.assignment_operator) = DIV_ASSIGN;
        }
    break;

  case 53:
#line 303 "zero.y"
    {
            (yyval.assignment_operator) = MOD_ASSIGN;
        }
    break;

  case 55:
#line 310 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(LOGICAL_OR_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 57:
#line 317 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(LOGICAL_AND_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 59:
#line 324 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(EQ_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 60:
#line 328 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(NE_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 62:
#line 335 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(GT_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 63:
#line 339 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(GE_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 64:
#line 343 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(LT_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 65:
#line 347 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(LE_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 67:
#line 354 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(ADD_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 68:
#line 358 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(SUB_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 70:
#line 365 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(MUL_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 71:
#line 369 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(DIV_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 72:
#line 373 "zero.y"
    {
            (yyval.expression) = zec_create_binary_expression(MOD_EXPRESSION, (yyvsp[-2].expression), (yyvsp[0].expression));
        }
    break;

  case 74:
#line 380 "zero.y"
    {
            (yyval.expression) = zec_create_minus_expression((yyvsp[0].expression));
        }
    break;

  case 75:
#line 384 "zero.y"
    {
            (yyval.expression) = zec_create_logical_not_expression((yyvsp[0].expression));
        }
    break;

  case 77:
#line 391 "zero.y"
    {
            (yyval.expression) = zec_create_incdec_expression((yyvsp[-1].expression), INCREMENT_EXPRESSION);
        }
    break;

  case 78:
#line 395 "zero.y"
    {
            (yyval.expression) = zec_create_incdec_expression((yyvsp[-1].expression), DECREMENT_EXPRESSION);
        }
    break;

  case 79:
#line 399 "zero.y"
    {
            (yyval.expression) = zec_create_instanceof_expression((yyvsp[-2].expression), (yyvsp[0].type_specifier));
        }
    break;

  case 82:
#line 407 "zero.y"
    {
            (yyval.expression) = zec_create_identifier_expression((yyvsp[0].identifier));
        }
    break;

  case 83:
#line 413 "zero.y"
    {
            (yyval.expression) = zec_create_index_expression((yyvsp[-3].expression), (yyvsp[-1].expression));
        }
    break;

  case 84:
#line 417 "zero.y"
    {
            Expression *identifier = zec_create_identifier_expression((yyvsp[-3].identifier));
            (yyval.expression) = zec_create_index_expression(identifier, (yyvsp[-1].expression));
        }
    break;

  case 85:
#line 422 "zero.y"
    {
            (yyval.expression) = zec_create_member_expression((yyvsp[-2].expression), (yyvsp[0].identifier));
        }
    break;

  case 86:
#line 426 "zero.y"
    {
            (yyval.expression) = zec_create_function_call_expression((yyvsp[-3].expression), (yyvsp[-1].argument_list));
        }
    break;

  case 87:
#line 430 "zero.y"
    {
            (yyval.expression) = zec_create_function_call_expression((yyvsp[-2].expression), NULL);
        }
    break;

  case 88:
#line 434 "zero.y"
    {
            (yyval.expression) = (yyvsp[-1].expression);
        }
    break;

  case 89:
#line 438 "zero.y"
    {
            (yyval.expression) = zec_create_down_cast_expression((yyvsp[-3].expression), (yyvsp[-1].type_specifier));
        }
    break;

  case 94:
#line 446 "zero.y"
    {
            (yyval.expression) = zec_create_boolean_expression(ZVM_TRUE);
        }
    break;

  case 95:
#line 450 "zero.y"
    {
            (yyval.expression) = zec_create_boolean_expression(ZVM_FALSE);
        }
    break;

  case 96:
#line 454 "zero.y"
    {
            (yyval.expression) = zec_create_null_expression();
        }
    break;

  case 98:
#line 459 "zero.y"
    {
            (yyval.expression) = zec_create_this_expression();
        }
    break;

  case 99:
#line 463 "zero.y"
    {
            (yyval.expression) = zec_create_super_expression();
        }
    break;

  case 100:
#line 467 "zero.y"
    {
            (yyval.expression) = zec_create_new_expression((yyvsp[-2].identifier), NULL, NULL);
        }
    break;

  case 101:
#line 471 "zero.y"
    {
            (yyval.expression) = zec_create_new_expression((yyvsp[-3].identifier), NULL, (yyvsp[-1].argument_list));
        }
    break;

  case 102:
#line 475 "zero.y"
    {
            (yyval.expression) = zec_create_new_expression((yyvsp[-4].identifier), (yyvsp[-2].identifier), NULL);
        }
    break;

  case 103:
#line 479 "zero.y"
    {
            (yyval.expression) = zec_create_new_expression((yyvsp[-5].identifier), (yyvsp[-3].identifier), (yyvsp[-1].argument_list));
        }
    break;

  case 104:
#line 485 "zero.y"
    {
            (yyval.expression) = zec_create_array_literal_expression((yyvsp[-1].expression_list));
        }
    break;

  case 105:
#line 489 "zero.y"
    {
            (yyval.expression) = zec_create_array_literal_expression((yyvsp[-2].expression_list));
        }
    break;

  case 106:
#line 495 "zero.y"
    {
            (yyval.expression) = zec_create_basic_array_creation((yyvsp[-1].basic_type_specifier), (yyvsp[0].array_dimension), NULL);
        }
    break;

  case 107:
#line 499 "zero.y"
    {
            (yyval.expression) = zec_create_basic_array_creation((yyvsp[-2].basic_type_specifier), (yyvsp[-1].array_dimension), (yyvsp[0].array_dimension));
        }
    break;

  case 108:
#line 503 "zero.y"
    {
            (yyval.expression) = zec_create_class_array_creation((yyvsp[-1].type_specifier), (yyvsp[0].array_dimension), NULL);
        }
    break;

  case 109:
#line 508 "zero.y"
    {
            (yyval.expression) = zec_create_class_array_creation((yyvsp[-2].type_specifier), (yyvsp[-1].array_dimension), (yyvsp[0].array_dimension));
        }
    break;

  case 111:
#line 515 "zero.y"
    {
            (yyval.array_dimension) = zec_chain_array_dimension((yyvsp[-1].array_dimension), (yyvsp[0].array_dimension));
        }
    break;

  case 112:
#line 521 "zero.y"
    {
            (yyval.array_dimension) = zec_create_array_dimension((yyvsp[-1].expression));
        }
    break;

  case 113:
#line 527 "zero.y"
    {
            (yyval.array_dimension) = zec_create_array_dimension(NULL);
        }
    break;

  case 114:
#line 531 "zero.y"
    {
            (yyval.array_dimension) = zec_chain_array_dimension((yyvsp[-2].array_dimension),
                                           zec_create_array_dimension(NULL));
        }
    break;

  case 115:
#line 538 "zero.y"
    {
            (yyval.expression_list) = NULL;
        }
    break;

  case 116:
#line 542 "zero.y"
    {
            (yyval.expression_list) = zec_create_expression_list((yyvsp[0].expression));
        }
    break;

  case 117:
#line 546 "zero.y"
    {
            (yyval.expression_list) = zec_chain_expression_list((yyvsp[-2].expression_list), (yyvsp[0].expression));
        }
    break;

  case 118:
#line 552 "zero.y"
    {
          (yyval.statement) = zec_create_expression_statement((yyvsp[-1].expression));
        }
    break;

  case 131:
#line 570 "zero.y"
    {
            (yyval.statement) = zec_create_if_statement((yyvsp[-2].expression), (yyvsp[0].block), NULL, NULL);
        }
    break;

  case 132:
#line 574 "zero.y"
    {
            (yyval.statement) = zec_create_if_statement((yyvsp[-4].expression), (yyvsp[-2].block), NULL, (yyvsp[0].block));
        }
    break;

  case 133:
#line 578 "zero.y"
    {
            (yyval.statement) = zec_create_if_statement((yyvsp[-3].expression), (yyvsp[-1].block), (yyvsp[0].elsif), NULL);
        }
    break;

  case 134:
#line 582 "zero.y"
    {
            (yyval.statement) = zec_create_if_statement((yyvsp[-5].expression), (yyvsp[-3].block), (yyvsp[-2].elsif), (yyvsp[0].block));
        }
    break;

  case 136:
#line 589 "zero.y"
    {
            (yyval.elsif) = zec_chain_elsif_list((yyvsp[-1].elsif), (yyvsp[0].elsif));
        }
    break;

  case 137:
#line 595 "zero.y"
    {
            (yyval.elsif) = zec_create_elsif((yyvsp[-2].expression), (yyvsp[0].block));
        }
    break;

  case 138:
#line 601 "zero.y"
    {
            (yyval.identifier) = NULL;
        }
    break;

  case 139:
#line 605 "zero.y"
    {
            (yyval.identifier) = (yyvsp[-1].identifier);
        }
    break;

  case 140:
#line 611 "zero.y"
    {
            (yyval.statement) = zec_create_switch_statement((yyvsp[-3].expression), (yyvsp[-1].case_list), (yyvsp[0].block));
        }
    break;

  case 142:
#line 618 "zero.y"
    {
            (yyval.case_list) = zec_chain_case((yyvsp[-1].case_list), (yyvsp[0].case_list));
        }
    break;

  case 143:
#line 624 "zero.y"
    {
            (yyval.case_list) = zec_create_one_case((yyvsp[-1].expression_list), (yyvsp[0].block));
        }
    break;

  case 144:
#line 630 "zero.y"
    {
            (yyval.block) = NULL;
        }
    break;

  case 145:
#line 634 "zero.y"
    {
            (yyval.block) = (yyvsp[0].block);
        }
    break;

  case 146:
#line 640 "zero.y"
    {
            (yyval.expression_list) = zec_create_expression_list((yyvsp[0].expression));
        }
    break;

  case 147:
#line 644 "zero.y"
    {
            (yyval.expression_list) = zec_chain_expression_list((yyvsp[-2].expression_list), (yyvsp[0].expression));
        }
    break;

  case 148:
#line 650 "zero.y"
    {
            (yyval.statement) = zec_create_while_statement((yyvsp[-5].identifier), (yyvsp[-2].expression), (yyvsp[0].block));
        }
    break;

  case 149:
#line 657 "zero.y"
    {
            (yyval.statement) = zec_create_for_statement((yyvsp[-9].identifier), (yyvsp[-6].expression), (yyvsp[-4].expression), (yyvsp[-2].expression), (yyvsp[0].block));
        }
    break;

  case 150:
#line 663 "zero.y"
    {
            (yyval.statement) = zec_create_do_while_statement((yyvsp[-7].identifier), (yyvsp[-5].block), (yyvsp[-2].expression));
        }
    break;

  case 151:
#line 669 "zero.y"
    {
            (yyval.statement) = zec_create_foreach_statement((yyvsp[-7].identifier), (yyvsp[-4].identifier), (yyvsp[-2].expression), (yyvsp[0].block));
        }
    break;

  case 152:
#line 675 "zero.y"
    {
            (yyval.expression) = NULL;
        }
    break;

  case 154:
#line 682 "zero.y"
    {
            (yyval.statement) = zec_create_return_statement((yyvsp[-1].expression));
        }
    break;

  case 155:
#line 688 "zero.y"
    {
            (yyval.identifier) = NULL;
        }
    break;

  case 157:
#line 695 "zero.y"
    {
            (yyval.statement) = zec_create_break_statement((yyvsp[-1].identifier));
        }
    break;

  case 158:
#line 701 "zero.y"
    {
            (yyval.statement) = zec_create_continue_statement((yyvsp[-1].identifier));
        }
    break;

  case 159:
#line 707 "zero.y"
    {
            (yyval.statement) = zec_create_try_statement((yyvsp[-3].block), (yyvsp[-2].catch_clause), (yyvsp[0].block));
        }
    break;

  case 160:
#line 711 "zero.y"
    {
            (yyval.statement) = zec_create_try_statement((yyvsp[-2].block), NULL, (yyvsp[0].block));
        }
    break;

  case 161:
#line 715 "zero.y"
    {
            (yyval.statement) = zec_create_try_statement((yyvsp[-1].block), (yyvsp[0].catch_clause), NULL);
        }
    break;

  case 163:
#line 722 "zero.y"
    {
            (yyval.catch_clause) = zec_chain_catch_list((yyvsp[-1].catch_clause), (yyvsp[0].catch_clause));
        }
    break;

  case 164:
#line 728 "zero.y"
    {
            (yyval.catch_clause) = zec_start_catch_clause();
        }
    break;

  case 165:
#line 732 "zero.y"
    {
            (yyval.catch_clause) = zec_end_catch_clause((yyvsp[-5].catch_clause), (yyvsp[-3].type_specifier), (yyvsp[-2].identifier), (yyvsp[0].block));
        }
    break;

  case 166:
#line 738 "zero.y"
    {
            (yyval.statement) = zec_create_throw_statement((yyvsp[-1].expression));
        }
    break;

  case 167:
#line 742 "zero.y"
    {
            (yyval.statement) = zec_create_throw_statement(NULL);
        }
    break;

  case 168:
#line 748 "zero.y"
    {
            (yyval.statement) = zec_create_declaration_statement(ZVM_FALSE, (yyvsp[-2].type_specifier), (yyvsp[-1].identifier), NULL);
        }
    break;

  case 169:
#line 752 "zero.y"
    {
            (yyval.statement) = zec_create_declaration_statement(ZVM_FALSE, (yyvsp[-4].type_specifier), (yyvsp[-3].identifier), (yyvsp[-1].expression));
        }
    break;

  case 170:
#line 756 "zero.y"
    {
            (yyval.statement) = zec_create_declaration_statement(ZVM_TRUE, (yyvsp[-2].type_specifier), (yyvsp[-1].identifier), NULL);
        }
    break;

  case 171:
#line 760 "zero.y"
    {
            (yyval.statement) = zec_create_declaration_statement(ZVM_TRUE, (yyvsp[-4].type_specifier), (yyvsp[-3].identifier), (yyvsp[-1].expression));
        }
    break;

  case 172:
#line 766 "zero.y"
    {
            (yyval.block) = zec_open_block();
        }
    break;

  case 173:
#line 770 "zero.y"
    {
            (yyval.block) = zec_close_block((yyvsp[-2].block), (yyvsp[-1].statement_list));
        }
    break;

  case 174:
#line 774 "zero.y"
    {
            Block *empty_block = zec_open_block();
            (yyval.block) = zec_close_block(empty_block, NULL);
        }
    break;

  case 175:
#line 782 "zero.y"
    {
            zec_start_class_definition(NULL, (yyvsp[-3].class_or_interface), (yyvsp[-2].identifier), (yyvsp[-1].extends_list));
        }
    break;

  case 176:
#line 786 "zero.y"
    {
            zec_class_define((yyvsp[-1].member_declaration));
        }
    break;

  case 177:
#line 791 "zero.y"
    {
            zec_start_class_definition(&(yyvsp[-4].class_or_member_modifier), (yyvsp[-3].class_or_interface), (yyvsp[-2].identifier), (yyvsp[-1].extends_list));
        }
    break;

  case 178:
#line 794 "zero.y"
    {
            zec_class_define((yyvsp[-1].member_declaration));
        }
    break;

  case 179:
#line 798 "zero.y"
    {
            zec_start_class_definition(NULL, (yyvsp[-3].class_or_interface), (yyvsp[-2].identifier), (yyvsp[-1].extends_list));
        }
    break;

  case 180:
#line 802 "zero.y"
    {
            zec_class_define(NULL);
        }
    break;

  case 181:
#line 807 "zero.y"
    {
            zec_start_class_definition(&(yyvsp[-4].class_or_member_modifier), (yyvsp[-3].class_or_interface), (yyvsp[-2].identifier), (yyvsp[-1].extends_list));
        }
    break;

  case 182:
#line 811 "zero.y"
    {
            zec_class_define(NULL);
        }
    break;

  case 184:
#line 818 "zero.y"
    {
            (yyval.class_or_member_modifier) = zec_chain_class_or_member_modifier((yyvsp[-1].class_or_member_modifier), (yyvsp[0].class_or_member_modifier));
        }
    break;

  case 186:
#line 825 "zero.y"
    {
            (yyval.class_or_member_modifier) = zec_create_class_or_member_modifier(VIRTUAL_MODIFIER);
        }
    break;

  case 187:
#line 829 "zero.y"
    {
            (yyval.class_or_member_modifier) = zec_create_class_or_member_modifier(OVERRIDE_MODIFIER);
        }
    break;

  case 188:
#line 833 "zero.y"
    {
            (yyval.class_or_member_modifier) = zec_create_class_or_member_modifier(ABSTRACT_MODIFIER);
        }
    break;

  case 189:
#line 839 "zero.y"
    {
            (yyval.class_or_interface) = ZVM_CLASS_DEFINITION;
        }
    break;

  case 190:
#line 843 "zero.y"
    {
            (yyval.class_or_interface) = ZVM_INTERFACE_DEFINITION;
        }
    break;

  case 191:
#line 849 "zero.y"
    {
            (yyval.extends_list) = NULL;
        }
    break;

  case 192:
#line 853 "zero.y"
    {
            (yyval.extends_list) = (yyvsp[0].extends_list);
        }
    break;

  case 193:
#line 859 "zero.y"
    {
            (yyval.extends_list) = zec_create_extends_list((yyvsp[0].identifier));
        }
    break;

  case 194:
#line 863 "zero.y"
    {
            (yyval.extends_list) = zec_chain_extends_list((yyvsp[-2].extends_list), (yyvsp[0].identifier));
        }
    break;

  case 196:
#line 870 "zero.y"
    {
            (yyval.member_declaration) = zec_chain_member_declaration((yyvsp[-1].member_declaration), (yyvsp[0].member_declaration));
        }
    break;

  case 199:
#line 880 "zero.y"
    {
            (yyval.member_declaration) = zec_create_method_member(NULL, (yyvsp[0].function_definition), ZVM_FALSE);
        }
    break;

  case 200:
#line 884 "zero.y"
    {
            (yyval.member_declaration) = zec_create_method_member(&(yyvsp[-1].class_or_member_modifier), (yyvsp[0].function_definition), ZVM_FALSE);
        }
    break;

  case 201:
#line 888 "zero.y"
    {
            (yyval.member_declaration) = zec_create_method_member(NULL, (yyvsp[0].function_definition), ZVM_TRUE);
        }
    break;

  case 202:
#line 892 "zero.y"
    {
            (yyval.member_declaration) = zec_create_method_member(&(yyvsp[-1].class_or_member_modifier), (yyvsp[0].function_definition), ZVM_TRUE);
        }
    break;

  case 203:
#line 898 "zero.y"
    {
            (yyval.function_definition) = zec_method_function_define((yyvsp[-6].type_specifier), (yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list), (yyvsp[0].block));
        }
    break;

  case 204:
#line 902 "zero.y"
    {
            (yyval.function_definition) = zec_method_function_define((yyvsp[-5].type_specifier), (yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list), (yyvsp[0].block));
        }
    break;

  case 205:
#line 907 "zero.y"
    {
            (yyval.function_definition) = zec_method_function_define((yyvsp[-6].type_specifier), (yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list), NULL);
        }
    break;

  case 206:
#line 911 "zero.y"
    {
            (yyval.function_definition) = zec_method_function_define((yyvsp[-5].type_specifier), (yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list), NULL);
        }
    break;

  case 207:
#line 917 "zero.y"
    {
            (yyval.exception_list) = NULL;
        }
    break;

  case 208:
#line 921 "zero.y"
    {
            (yyval.exception_list) = (yyvsp[0].exception_list);
        }
    break;

  case 209:
#line 927 "zero.y"
    {
            (yyval.exception_list) = zec_create_throws((yyvsp[0].identifier));
        }
    break;

  case 210:
#line 931 "zero.y"
    {
            (yyval.exception_list) = zec_chain_exception_list((yyvsp[-2].exception_list), (yyvsp[0].identifier));
        }
    break;

  case 211:
#line 937 "zero.y"
    {
            (yyval.function_definition) = zec_constructor_function_define((yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list), (yyvsp[0].block));
        }
    break;

  case 212:
#line 941 "zero.y"
    {
            (yyval.function_definition) = zec_constructor_function_define((yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list), (yyvsp[0].block));
        }
    break;

  case 213:
#line 945 "zero.y"
    {
            (yyval.function_definition) = zec_constructor_function_define((yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list), NULL);
        }
    break;

  case 214:
#line 949 "zero.y"
    {
            (yyval.function_definition) = zec_constructor_function_define((yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list), NULL);
        }
    break;

  case 215:
#line 955 "zero.y"
    {
            (yyval.class_or_member_modifier) = zec_create_class_or_member_modifier(PUBLIC_MODIFIER);
        }
    break;

  case 216:
#line 959 "zero.y"
    {
            (yyval.class_or_member_modifier) = zec_create_class_or_member_modifier(PRIVATE_MODIFIER);
        }
    break;

  case 217:
#line 965 "zero.y"
    {
            (yyval.member_declaration) = zec_create_field_member(NULL, ZVM_FALSE, (yyvsp[-2].type_specifier), (yyvsp[-1].identifier));
        }
    break;

  case 218:
#line 970 "zero.y"
    {
            (yyval.member_declaration) = zec_create_field_member(&(yyvsp[-3].class_or_member_modifier), ZVM_FALSE, (yyvsp[-2].type_specifier), (yyvsp[-1].identifier));
        }
    break;

  case 219:
#line 974 "zero.y"
    {
            (yyval.member_declaration) = zec_create_field_member(NULL, ZVM_TRUE, (yyvsp[-2].type_specifier), (yyvsp[-1].identifier));
        }
    break;

  case 220:
#line 979 "zero.y"
    {
            (yyval.member_declaration) = zec_create_field_member(&(yyvsp[-4].class_or_member_modifier), ZVM_TRUE, (yyvsp[-2].type_specifier), (yyvsp[-1].identifier));
        }
    break;

  case 221:
#line 986 "zero.y"
    {
            zec_create_delegate_definition((yyvsp[-6].type_specifier), (yyvsp[-5].identifier), (yyvsp[-3].parameter_list), (yyvsp[-1].exception_list));
        }
    break;

  case 222:
#line 990 "zero.y"
    {
            zec_create_delegate_definition((yyvsp[-5].type_specifier), (yyvsp[-4].identifier), NULL, (yyvsp[-1].exception_list));
        }
    break;

  case 223:
#line 996 "zero.y"
    {
            zec_create_enum_definition((yyvsp[-4].identifier), (yyvsp[-2].enumerator));
        }
    break;

  case 224:
#line 1000 "zero.y"
    {
            zec_create_enum_definition((yyvsp[-5].identifier), (yyvsp[-3].enumerator));
        }
    break;

  case 225:
#line 1006 "zero.y"
    {
            (yyval.enumerator) = zec_create_enumerator((yyvsp[0].identifier));
        }
    break;

  case 226:
#line 1010 "zero.y"
    {
            (yyval.enumerator) = zec_chain_enumerator((yyvsp[-2].enumerator), (yyvsp[0].identifier));
        }
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 3041 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1014 "zero.y"


