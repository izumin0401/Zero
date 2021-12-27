#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "DBG.h"
#include "MEM.h"
#include "ZVM.h"

size_t
zvm_wcslen(wchar_t *str)
{
    return wcslen(str);
}

wchar_t *
zvm_wcscpy(wchar_t *dest, wchar_t *src)
{
    return wcscpy(dest, src);
}

wchar_t *
zvm_wcsncpy(wchar_t *dest, wchar_t *src, size_t n)
{
    return wcsncpy(dest, src, n);
}

int
zvm_wcscmp(wchar_t *s1, wchar_t *s2)
{
    return wcscmp(s1, s2);
}

wchar_t *
zvm_wcscat(wchar_t *s1, wchar_t *s2)
{
    return wcscat(s1, s2);
}

int
zvm_mbstowcs_len(const char *src)
{
    int src_idx, dest_idx;
    int status;
    mbstate_t ps;

    memset(&ps, 0, sizeof(mbstate_t));
    for (src_idx = dest_idx = 0; src[src_idx] != '\0'; ) {
        status = mbrtowc(NULL, &src[src_idx], 2, &ps);
        if (status < 0) {
            return status;
        }
        dest_idx++;
        src_idx += status;
    }

    return dest_idx;
}

void
zvm_mbstowcs(const char *src, wchar_t *dest)
{
    int src_idx, dest_idx;
    int status;
    mbstate_t ps;

    memset(&ps, 0, sizeof(mbstate_t));
    for (src_idx = dest_idx = 0; src[src_idx] != '\0'; ) {
        status = mbrtowc(&dest[dest_idx], &src[src_idx], 2, &ps);
        dest_idx++;
        src_idx += status;
    }
    dest[dest_idx] = L'\0';
}

int
zvm_wcstombs_len(const wchar_t *src)
{
    int src_idx, dest_idx;
    int status;
    char dummy[2];
    mbstate_t ps;

    memset(&ps, 0, sizeof(mbstate_t));
    for (src_idx = dest_idx = 0; src[src_idx] != L'\0'; ) {
        status = wcrtomb(dummy, src[src_idx], &ps);
        src_idx++;
        dest_idx += status;
    }

    return dest_idx;
}

void
zvm_wcstombs_i(const wchar_t *src, char *dest)
{
    int src_idx, dest_idx;
    int status;
    mbstate_t ps;

    memset(&ps, 0, sizeof(mbstate_t));
    for (src_idx = dest_idx = 0; src[src_idx] != '\0'; ) {
        status = wcrtomb(&dest[dest_idx], src[src_idx], &ps);
        src_idx++;
        dest_idx += status;
    }
    dest[dest_idx] = '\0';
}

char *
zvm_wcstombs_alloc(const wchar_t *src)
{
    int len;
    char *ret;

    len = zvm_wcstombs_len(src);
    ret = MEM_malloc(len + 1);
    zvm_wcstombs_i(src, ret);

    return ret;
}

char
zvm_wctochar(wchar_t src)
{
    mbstate_t ps;
    int status;
    char dest;

    memset(&ps, 0, sizeof(mbstate_t));
    status = wcrtomb(&dest, src, &ps);
    DBG_assert(status == 1, ("wcrtomb status..%d\n", status));

    return dest;
}

int
zvm_print_wcs(FILE *fp, wchar_t *str)
{
    char *tmp;
    int result;

    tmp = zvm_wcstombs_alloc(str);
    result = fprintf(fp, "%s", tmp);
    MEM_free(tmp);

    return result;
}


int
zvm_print_wcs_ln(FILE *fp, wchar_t *str)
{
    int result;

    result = zvm_print_wcs(fp, str);
    fprintf(fp, "\n");

    return result;
}

ZVM_Boolean
zvm_iswdigit(wchar_t ch)
{
    return ch >= L'0' && ch <= L'9';
}
