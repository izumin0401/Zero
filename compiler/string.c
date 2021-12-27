#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "zeroc.h"

#define STRING_ALLOC_SIZE       (256)

static char *st_string_literal_buffer = NULL;
static int st_string_literal_buffer_size = 0;
static int st_string_literal_buffer_alloc_size = 0;

void
zec_open_string_literal(void)
{
    st_string_literal_buffer_size = 0;
}

void
zec_add_string_literal(int letter)
{
    if (st_string_literal_buffer_size == st_string_literal_buffer_alloc_size) {
        st_string_literal_buffer_alloc_size += STRING_ALLOC_SIZE;
        st_string_literal_buffer
            = MEM_realloc(st_string_literal_buffer,
                          st_string_literal_buffer_alloc_size);
    }
    st_string_literal_buffer[st_string_literal_buffer_size] = letter;
    st_string_literal_buffer_size++;
}

void
zec_reset_string_literal_buffer(void)
{
    MEM_free(st_string_literal_buffer);
    st_string_literal_buffer = NULL;
    st_string_literal_buffer_size = 0;
    st_string_literal_buffer_alloc_size = 0;
}

ZVM_Char *
zec_close_string_literal(void)
{
    ZVM_Char *new_str;
    int new_str_len;

    zec_add_string_literal('\0');
    new_str_len = zvm_mbstowcs_len(st_string_literal_buffer);
    if (new_str_len < 0) {
        zec_compile_error(zec_get_current_compiler()->current_line_number,
                          BAD_MULTIBYTE_CHARACTER_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    new_str = MEM_malloc(sizeof(ZVM_Char) * (new_str_len+1));
    zvm_mbstowcs(st_string_literal_buffer, new_str);

    return new_str;
}

char *
zec_create_identifier(char *str)
{
    char *new_str;

    new_str = zec_malloc(strlen(str) + 1);

    strcpy(new_str, str);

    return new_str;
}
